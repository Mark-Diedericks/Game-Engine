#include "ge.h"
#include "Common.h"
#include "GLShader.h"

#include <GL/glew.h>
#include "System/Memory.h"
#include "System/VirtualFileSystem.h"

namespace gebase { namespace graphics { namespace API {

	enum class ShaderType
	{
		UNKNOWN,
		VERTEX,
		FRAGMENT
	};

	GLShader::GLShader(const String& name, const String& source) : m_Name(name), m_Source(source)
	{
		Init();
	}

	GLShader::~GLShader()
	{
		Shutdown();
	}

	void GLShader::Init()
	{
		m_VSUserUniformBuffer = nullptr;
		m_FSUserUniformBuffer = nullptr;

		String* shaders[2] = { &m_VertexSource, &m_FragmentSource };
		Preprocess(m_Source, shaders);
		Parse(m_VertexSource, m_FragmentSource);

		GLShaderErrorInfo info;
		m_Handle = Compile(shaders, info);

		if (!m_Handle)
		{
			std::cout << "[GLShader] Error: " << info.message[info.shader].c_str() << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		ResolveUniforms();
		ValidateUniforms();
	}

	void GLShader::Shutdown()
	{
		GLCall(glDeleteProgram(m_Handle));
	}

	void GLShader::Preprocess(const String& source, String** shaders)
	{
		ShaderType type = ShaderType::UNKNOWN;

		std::vector<String> lines = GetLines(source);

		for (uint i = 0; i < lines.size(); i++)
		{
			const char* str = lines[i].c_str();

			if (strstr(str, "#shader"))
			{
				if (strstr(str, "vertex"))
					type = ShaderType::VERTEX;
				else if (strstr(str, "fragment"))
					type = ShaderType::FRAGMENT;
			}
			else if (type != ShaderType::UNKNOWN)
			{
				shaders[(int32)type - 1]->append(str);
				shaders[(int32)type - 1]->append("\n");
			}
		}
	}

	uint GLShader::Compile(String** shaders, GLShaderErrorInfo& info)
	{
		const char* vsSource = shaders[0]->c_str();
		const char* fsSource = shaders[1]->c_str();

		GLint result;

		GLCall(uint program = glCreateProgram());
		GLCall(GLuint vsID = glCreateShader(GL_VERTEX_SHADER));
		GLCall(GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER));

		GLCall(glShaderSource(vsID, 1, &vsSource, NULL));
		GLCall(glCompileShader(vsID));

		GLCall(glGetShaderiv(vsID, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			GLint length;
			GLCall(glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &length));

			std::vector<char> error(length);
			GLCall(glGetShaderInfoLog(vsID, length, &length, &error[0]));

			String errorMsg(&error[0]);
			info.shader = 0;
			info.message[info.shader] += "Failed to compiled the vertex shader.\n";
			info.message[info.shader] += errorMsg;

			GLCall(glDeleteShader(vsID));
			return 0;
		}

		GLCall(glShaderSource(fsID, 1, &fsSource, NULL));
		GLCall(glCompileShader(fsID));

		GLCall(glGetShaderiv(fsID, GL_COMPILE_STATUS, &result));
		if(result == GL_FALSE)
		{
			GLint length;
			GLCall(glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &length));

			std::vector<char> error(length);
			GLCall(glGetShaderInfoLog(fsID, length, &length, &error[0]));

			String errorMsg(&error[0], length);
			int32 lineNum;
			sscanf(&error[0], "%*s %*d:%d", &lineNum);

			info.shader = 1;
			info.line[info.shader] = lineNum;
			info.message[info.shader] += "Failed to compile the fragment shader.\n";
			info.message[info.shader] += errorMsg;

			GLCall(glDeleteShader(fsID));

			return 0;
		}

		GLCall(glAttachShader(program, vsID));
		GLCall(glAttachShader(program, fsID));

		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDetachShader(program, vsID));
		GLCall(glDetachShader(program, fsID));

		GLCall(glDeleteShader(vsID));
		GLCall(glDeleteShader(fsID));

		return program;
	}

	bool GLShader::TryCompile(const String& source, String& error)
	{
		String vert;
		String frag;
		String* shaders[2] = { &vert, &frag };
		
		GLShader::Preprocess(source, shaders);
		GLShaderErrorInfo info;

		if (!GLShader::Compile(shaders, info))
		{
			error = info.message[info.shader];
			return false;
		}

		return true;
	}

	void GLShader::Bind() const
	{
		GLCall(glUseProgram(m_Handle));
		s_CurrentlyBound = this;
	}
	
	void GLShader::Unbind() const
	{
		GLCall(glUseProgram(0));
		s_CurrentlyBound = nullptr;
	}
	
	void GLShader::Parse(const String& vsSource, const String& fsSource)
	{
		m_VSUniformBuffers.push_back(genew GLShaderUniformBufferDeclaration("Global", 0));
		m_FSUniformBuffers.push_back(genew GLShaderUniformBufferDeclaration("Global", 1));

		const char* token;
		const char* vstr;
		const char* fstr;

		//Vertex Shader structs
		vstr = vsSource.c_str();
		while (token = FindToken(vstr, "struct"))
			ParseUniformStruct(GetBlock(token, &vstr), 0);

		//Vertex Shader uniforms
		vstr = vsSource.c_str();
		while (token = FindToken(vstr, "uniform"))
			ParseUniform(GetStatement(token, &vstr), 0);

		//Fragment Shader structs
		fstr = fsSource.c_str();
		while (token = FindToken(fstr, "struct"))
			ParseUniformStruct(GetBlock(token, &fstr), 1);

		//Fragment Shader uniforms
		fstr = fsSource.c_str();
		while (token = FindToken(fstr, "uniform"))
			ParseUniform(GetStatement(token, &fstr), 1);
	}

	void GLShader::ParseUniform(const String& statement, uint shaderType)
	{
		std::vector<String> tokens = Tokenize(statement);
		uint index = 0;

		index++; //Skip "uniform"

		String typeString = tokens[index++];
		String name = tokens[index++];

		if (const char* s = strstr(name.c_str(), ";"))
			name = String(name.c_str(), s - name.c_str());

		String n(name);
		int32 count = 1;
		
		const char* namestr = n.c_str();
		if (const char* s = strstr(namestr, "["))
		{
			name = String(namestr, s - namestr);

			const char* end = strstr(namestr, "]");
			String c(s + 1, end - s);
			count = atoi(c.c_str());
		}

		if (isTypeStringResource(typeString))
		{
			ShaderResourceDeclaration* declaration = new GLShaderResourceDeclaration(GLShaderResourceDeclaration::StringToType(typeString), name, count);
			m_Resources.push_back(declaration);
		}
		else
		{
			GLShaderUniformDeclaration::UniformType t = GLShaderUniformDeclaration::StringToType(typeString);
			GLShaderUniformDeclaration* declaration = nullptr;

			if (t == GLShaderUniformDeclaration::UniformType::NONE)
			{
				ShaderStruct* s = FindStruct(typeString);

				if (!s)
				{
					std::cout << "[GLShader] ParseUniform() - Shader Struct parsing is not implemented" << std::endl;
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}
				declaration = new GLShaderUniformDeclaration(s, name, count);
			}
			else
			{
				declaration = new GLShaderUniformDeclaration(t, name, count);
			}

			if (StartsWith(name, "sys_"))
			{
				if (shaderType == 0)
					((GLShaderUniformBufferDeclaration*)m_VSUniformBuffers.front())->PushUniform(declaration);
				else if (shaderType == 1)
					((GLShaderUniformBufferDeclaration*)m_FSUniformBuffers.front())->PushUniform(declaration);
			}
			else
			{
				if (shaderType == 0)
				{
					if (m_VSUserUniformBuffer == nullptr)
						m_VSUserUniformBuffer = new GLShaderUniformBufferDeclaration("", 0);

					m_VSUserUniformBuffer->PushUniform(declaration);
				}
				else if (shaderType == 1)
				{
					if (m_FSUserUniformBuffer == nullptr)
						m_FSUserUniformBuffer = new GLShaderUniformBufferDeclaration("", 0);

					m_FSUserUniformBuffer->PushUniform(declaration);
				}
			}
		}
	}

	void GLShader::ParseUniformStruct(const String& block, uint shaderType)
	{
		std::vector<String> tokens = Tokenize(block);

		uint index = 0;
		index++; //Skip "struct"

		String name = tokens[index++];
		ShaderStruct* uniformStruct = genew ShaderStruct(name);
		index++; //Skip "{"

		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			String type = tokens[index++];
			String name = tokens[index++];

			if (const char* s = strstr(name.c_str(), ";"))
				name = String(name.c_str(), s - name.c_str());

			uint count = 1;
			const char* namestr = name.c_str();

			if (const char* s = strstr(namestr, "["))
			{
				name = String(namestr, s - namestr);

				const char* end = strstr(namestr, "]");
				String c(s + 1, end - s);
				count = atoi(c.c_str());
			}

			ShaderUniformDeclaration* field = genew GLShaderUniformDeclaration(GLShaderUniformDeclaration::StringToType(type), name, count);
			uniformStruct->AddField(field);
		}

		m_Structs.push_back(uniformStruct);
	}

	bool GLShader::isTypeStringResource(const String& type)
	{
		return type == "sampler2D" || type == "samplerCube" || type == "sampler2DShadow";
	}

	bool GLShader::isSystemUniform(ShaderUniformDeclaration* uniform) const
	{
		return StartsWith(uniform->getName(), "sys_");
	}

	void GLShader::ResolveUniforms()
	{
		Bind();

		for (uint i = 0; i < m_VSUniformBuffers.size(); i++)
		{
			GLShaderUniformBufferDeclaration* decl = (GLShaderUniformBufferDeclaration*)m_VSUniformBuffers[i];
			const ShaderUniformList& uniforms = decl->getUniformDeclarations();

			for (uint j = 0; j < uniforms.size(); j++)
			{
				GLShaderUniformDeclaration* uniform = (GLShaderUniformDeclaration*)uniforms[j];

				if (uniform->getType() == GLShaderUniformDeclaration::UniformType::STRUCT)
				{
					const ShaderStruct& s = uniform->getShaderUniformStruct();
					const auto& fields = s.getFields();

					for (uint k = 0; k < fields.size(); k++)
					{
						GLShaderUniformDeclaration* field = (GLShaderUniformDeclaration*)fields[k];
						field->m_Location = getUniformLocation(uniform->m_Name + "." + field->m_Name);
					}
				}
				else
				{
					uniform->m_Location = getUniformLocation(uniform->m_Name);
				}
			}
		}

		for (uint i = 0; i < m_FSUniformBuffers.size(); i++)
		{
			GLShaderUniformBufferDeclaration* decl = (GLShaderUniformBufferDeclaration*)m_FSUniformBuffers[i];
			const ShaderUniformList& uniforms = decl->getUniformDeclarations();

			for (uint j = 0; j < uniforms.size(); j++)
			{
				GLShaderUniformDeclaration* uniform = (GLShaderUniformDeclaration*)uniforms[j];

				if (uniform->getType() == GLShaderUniformDeclaration::UniformType::STRUCT)
				{
					const ShaderStruct& s = uniform->getShaderUniformStruct();
					const auto& fields = s.getFields();

					for (uint k = 0; k < fields.size(); k++)
					{
						GLShaderUniformDeclaration* field = (GLShaderUniformDeclaration*)fields[k];
						field->m_Location = getUniformLocation(uniform->m_Name + "." + field->m_Name);
					}
				}
				else
				{
					uniform->m_Location = getUniformLocation(uniform->m_Name);
				}
			}
		}

		{
			GLShaderUniformBufferDeclaration* decl = m_VSUserUniformBuffer;
			if (decl)
			{
				const ShaderUniformList& uniforms = decl->getUniformDeclarations();

				for (uint j = 0; j < uniforms.size(); j++)
				{
					GLShaderUniformDeclaration* uniform = (GLShaderUniformDeclaration*)uniforms[j];

					if (uniform->getType() == GLShaderUniformDeclaration::UniformType::STRUCT)
					{
						const ShaderStruct& s = uniform->getShaderUniformStruct();
						const auto& fields = s.getFields();

						for (uint k = 0; k < fields.size(); k++)
						{
							GLShaderUniformDeclaration* field = (GLShaderUniformDeclaration*)fields[k];
							field->m_Location = getUniformLocation(uniform->m_Name + "." + field->m_Name);
						}
					}
					else
					{
						uniform->m_Location = getUniformLocation(uniform->m_Name);
					}
				}
			}
		}

		{
			GLShaderUniformBufferDeclaration* decl = m_FSUserUniformBuffer;
			if (decl)
			{
				const ShaderUniformList& uniforms = decl->getUniformDeclarations();

				for (uint j = 0; j < uniforms.size(); j++)
				{
					GLShaderUniformDeclaration* uniform = (GLShaderUniformDeclaration*)uniforms[j];

					if (uniform->getType() == GLShaderUniformDeclaration::UniformType::STRUCT)
					{
						const ShaderStruct& s = uniform->getShaderUniformStruct();
						const auto& fields = s.getFields();

						for (uint k = 0; k < fields.size(); k++)
						{
							GLShaderUniformDeclaration* field = (GLShaderUniformDeclaration*)fields[k];
							field->m_Location = getUniformLocation(uniform->m_Name + "." + field->m_Name);
						}
					}
					else
					{
						uniform->m_Location = getUniformLocation(uniform->m_Name);
					}
				}
			}
		}

		uint sampler = 0;
		for (uint i = 0; i < m_Resources.size(); i++)
		{
			GLShaderResourceDeclaration* resource = (GLShaderResourceDeclaration*)m_Resources[i];
			uint location = getUniformLocation(resource->m_Name);

			if (resource->getCount() == 1)
			{
				resource->m_Register = sampler;
				setUniform1i(location, sampler++);
			}
			else if(resource->getCount() > 1)
			{
				resource->m_Register = 0;
				uint count = resource->getCount();
				int32* samplers = genew int32[count];

				for (uint j = 0; j < count; j++)
					samplers[j] = j;

				setUniform1iv(resource->getName(), samplers, count);
				gedel[] samplers;

			}
		}

		Unbind();
	}

	void GLShader::ValidateUniforms()
	{
		std::cout << "[GLShader] ValidateUniforms() - Not implemented" << std::endl;
	}

	GLint GLShader::getUniformLocation(const String& name)
	{
		GLCall(GLint result = glGetUniformLocation(m_Handle, name.c_str()));

		if (result == -1)
			std::cout << "[GLShader] getUniformLocation() - Could not find uniform: " << name.c_str() << std::endl;

		return result;
	}

	ShaderUniformDeclaration* GLShader::FindUniformDeclaration(const String& name)
	{
		ShaderUniformDeclaration* result = nullptr;

		for (uint i = 0; i < m_VSUniformBuffers.size(); i++)
		{
			result = FindUniformDeclaration(name, m_VSUniformBuffers[i]);
			if (result) return result;
		}

		for (uint i = 0; i < m_FSUniformBuffers.size(); i++)
		{
			result = FindUniformDeclaration(name, m_FSUniformBuffers[i]);
			if (result) return result;
		}

		result = FindUniformDeclaration(name, m_VSUserUniformBuffer);
		if (result) return result;

		result = FindUniformDeclaration(name, m_FSUserUniformBuffer);
		if (result) return result;

		return result;
	}

	ShaderUniformDeclaration* GLShader::FindUniformDeclaration(const String& name, const ShaderUniformBufferDeclaration* buffer)
	{
		const ShaderUniformList& uniforms = buffer->getUniformDeclarations();

		for (uint i = 0; i < uniforms.size(); i++)
		{
			if (uniforms[i]->getName() == name)
				return uniforms[i];
		}

		return nullptr;
	}

	ShaderStruct* GLShader::FindStruct(const String& name)
	{
		for (ShaderStruct* s : m_Structs)
		{
			if (s->getName() == name)
				return s;
		}

		return nullptr;
	}

	void GLShader::setVSSystemUniformBuffer(byte* data, uint size, uint slot)
	{
		Bind();

		if (!(m_VSUniformBuffers.size() > slot))
		{
			std::cout << "[GLShader] setVSSystemUniformBuffer() - Size is not greater than slot." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		ShaderUniformBufferDeclaration* declaration = m_VSUniformBuffers[slot];
		ResolveAndSetUniforms(declaration, data, size);
	}

	void GLShader::setFSSystemUniformBuffer(byte* data, uint size, uint slot)
	{

		Bind();

		if (!(m_FSUniformBuffers.size() > slot))
		{
			std::cout << "[GLShader] setFSSystemUniformBuffer() - Size is not greater than slot." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		ShaderUniformBufferDeclaration* declaration = m_FSUniformBuffers[slot];
		ResolveAndSetUniforms(declaration, data, size);
	}

	void GLShader::setVSUserUniformBuffer(byte* data, uint size)
	{
		ResolveAndSetUniforms(m_VSUserUniformBuffer, data, size);
	}

	void GLShader::setFSUserUniformBuffer(byte* data, uint size)
	{
		ResolveAndSetUniforms(m_FSUserUniformBuffer, data, size);
	}

	void GLShader::setUniformStruct(GLShaderUniformDeclaration* uniform, byte* data, int32 offset)
	{
		const ShaderStruct& s = uniform->getShaderUniformStruct();
		const auto& fields = s.getFields();

		for (uint i = 0; i < fields.size(); i++)
		{
			GLShaderUniformDeclaration* field = (GLShaderUniformDeclaration*)fields[i];
			ResolveAndSetUniformField(*field, data, offset);
			offset += field->getSize();
		}
	}

	void GLShader::ResolveAndSetUniforms(ShaderUniformBufferDeclaration* buffer, byte* data, uint size)
	{
		const ShaderUniformList& uniforms = buffer->getUniformDeclarations();

		for (uint i = 0; i < uniforms.size(); i++)
		{
			GLShaderUniformDeclaration* uniform = (GLShaderUniformDeclaration*)uniforms[i];
			ResolveAndSetUniform(uniform, data, size);
		}
	}

	void GLShader::ResolveAndSetUniform(GLShaderUniformDeclaration* uniform, byte* data, uint size)
	{
		if (uniform->getLocation() == -1)
			return;

		uint offset = uniform->getOffset();

		switch (uniform->getType())
		{
		case GLShaderUniformDeclaration::UniformType::FLOAT32:
			setUniform1f(uniform->getLocation(), *(float*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::INT32:
			setUniform1i(uniform->getLocation(), *(int32*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC2:
			setUniform2f(uniform->getLocation(), *(math::Vector2f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC3:
			setUniform3f(uniform->getLocation(), *(math::Vector3f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC4:
			setUniform4f(uniform->getLocation(), *(math::Vector4f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::MAT3:
			//TODO Implement    setUniformMat3(uniform->getLocation(), *(math::Matrix3f*)&data[offset]);
			std::cout << "[GLShader] ResolveAndSetUniform() - Cannot set MAT3 uniform type, unimplemented." << std::endl;
			break;
		case GLShaderUniformDeclaration::UniformType::MAT4:
			setUniformMat4(uniform->getLocation(), *(math::Matrix4f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::STRUCT:
			break;
		default:
			std::cout << "[GLShader] ResolveAndSetUniform() - Unknown uniform type." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
			break;
		}
	}

	void GLShader::ResolveAndSetUniformField(const GLShaderUniformDeclaration& field, byte* data, int32 offset)
	{
		switch (field.getType())
		{
		case GLShaderUniformDeclaration::UniformType::FLOAT32:
			setUniform1f(field.getLocation(), *(float*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::INT32:
			setUniform1i(field.getLocation(), *(int32*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC2:
			setUniform2f(field.getLocation(), *(math::Vector2f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC3:
			setUniform3f(field.getLocation(), *(math::Vector3f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::VEC4:
			setUniform4f(field.getLocation(), *(math::Vector4f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::MAT3:
			//TODO Implement    setUniformMat3(field.getLocation(), *(math::Matrix3f*)&data[offset]);
			std::cout << "[GLShader] ResolveAndSetUniformField() - Cannot set MAT3 uniform type, unimplemented." << std::endl;
			break;
		case GLShaderUniformDeclaration::UniformType::MAT4:
			setUniformMat4(field.getLocation(), *(math::Matrix4f*)&data[offset]);
			break;
		case GLShaderUniformDeclaration::UniformType::STRUCT:
			break;
		default:
			std::cout << "[GLShader] ResolveAndSetUniform() - Unknown uniform type." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
			break;
		}
	}

	void GLShader::setUniform(const String& name, byte* data)
	{
		ShaderUniformDeclaration* uniform = FindUniformDeclaration(name);

		if (!uniform)
		{
			std::cout << "[GLShader] setUniform() - Could not find uniform declaration." << std::endl;
			return;
		}

		ResolveAndSetUniform((GLShaderUniformDeclaration*)uniform, data, 0);
	}

	void GLShader::setUniform1f(const String& name, float value) 
	{
		setUniform1f(getUniformLocation(name), value);
	}

	void GLShader::setUniform1fv(const String& name, float* value, int32 count)
	{
		setUniform1fv(getUniformLocation(name), value, count);
	}

	void GLShader::setUniform1i(const String& name, int32 value)
	{
		setUniform1i(getUniformLocation(name), value);
	}

	void GLShader::setUniform1iv(const String& name, int32* value, int32 count)
	{
		setUniform1iv(getUniformLocation(name), value, count);
	}

	void GLShader::setUniform2f(const String& name, math::Vector2f& value)
	{
		setUniform2f(getUniformLocation(name), value);
	}

	void GLShader::setUniform3f(const String& name, math::Vector3f& value)
	{
		setUniform3f(getUniformLocation(name), value);
	}

	void GLShader::setUniform4f(const String& name, math::Vector4f& value)
	{
		setUniform4f(getUniformLocation(name), value);
	}

	void GLShader::setUniformMat4(const String& name, math::Matrix4f& value)
	{
		setUniformMat4(getUniformLocation(name), value);
	}

	void GLShader::setUniform1f(uint location, float value)
	{
		GLCall(glUniform1f(location, value));
	}

	void GLShader::setUniform1fv(uint location, float* value, int32 count)
	{
		GLCall(glUniform1fv(location, count, value));
	}

	void GLShader::setUniform1i(uint location, int32 value)
	{
		GLCall(glUniform1i(location, value));
	}

	void GLShader::setUniform1iv(uint location, int32* value, int32 count)
	{
		GLCall(glUniform1iv(location, count, value));
	}

	void GLShader::setUniform2f(uint location, math::Vector2f& value)
	{
		GLCall(glUniform2f(location, value.x, value.y));
	}

	void GLShader::setUniform3f(uint location, math::Vector3f& value)
	{
		GLCall(glUniform3f(location, value.x, value.y, value.z));
	}

	void GLShader::setUniform4f(uint location, math::Vector4f& value)
	{
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void GLShader::setUniformMat4(uint location, math::Matrix4f& value)
	{
		GLCall(glUniformMatrix4fv(location, 1, GL_TRUE, value.elements));
	}

} } }