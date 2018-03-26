#include "ge.h"
#include "GLShaderUniform.h"

#include "Graphics/Shader/Shader.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	GLShaderUniformDeclaration::GLShaderUniformDeclaration(UniformType type, const String& name, uint count) : m_Type(type), m_Name(name), m_Count(count) 
	{ 
		m_Size = SizeOfUniformType(type) * count;
	}

	GLShaderUniformDeclaration::GLShaderUniformDeclaration(ShaderStruct* uniformStruct, const String& name, uint count) : m_Type(UniformType::STRUCT), m_Struct(uniformStruct), m_Name(name), m_Count(count) 
	{ 
		m_Size = m_Struct->getSize() * count; 
	}

	GLShaderUniformDeclaration::~GLShaderUniformDeclaration()
	{
		gedel m_Struct;
	}

	void GLShaderUniformDeclaration::setOffset(uint offset)
	{
		if (m_Type == UniformType::STRUCT)
			m_Struct->setOffset(offset);

		m_Offset = offset;
	}

	uint GLShaderUniformDeclaration::SizeOfUniformType(UniformType type)
	{
		switch (type)
		{
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::FLOAT32:	return 4 * 1 * 1; //Bytes for a float multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::INT32:		return 4 * 1 * 1; //Bytes for a int32 multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC2:		return 4 * 2 * 1; //Bytes for a float multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC3:		return 4 * 3 * 1; //Bytes for a float multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC4:		return 4 * 4 * 1; //Bytes for a float multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::MAT3:		return 4 * 3 * 3; //Bytes for a float multiplied by the count in two dimensions
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::MAT4:		return 4 * 4 * 4; //Bytes for a float multiplied by the count in two dimensions
		}

		return 0;
	}

	GLShaderUniformDeclaration::UniformType GLShaderUniformDeclaration::StringToType(const String& type)
	{
		if (type == "float") return UniformType::FLOAT32;
		if (type == "int32") return UniformType::INT32;
		if (type == "vec2") return UniformType::VEC2;
		if (type == "vec3") return UniformType::VEC3;
		if (type == "vec4") return UniformType::VEC4;
		if (type == "mat3") return UniformType::MAT3;
		if (type == "mat4") return UniformType::MAT4;

		return UniformType::NONE;
	}

	String GLShaderUniformDeclaration::TypeToString(UniformType type)
	{
		switch (type)
		{
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::FLOAT32:	return "float";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::INT32:		return "in32";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC2:		return "vec2";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC3:		return "vec3";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::VEC4:		return "vec4";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::MAT3:		return "mat3";
		case gebase::graphics::GLShaderUniformDeclaration::UniformType::MAT4:		return "mat4";
		}

		return "Invalid Type";
	}

	GLShaderUniformBufferDeclaration::GLShaderUniformBufferDeclaration(const String& name, uint shaderType) : m_Name(name), m_ShaderType(shaderType), m_Size(0), m_Register(0) 
	{
		
	}

	GLShaderUniformBufferDeclaration::~GLShaderUniformBufferDeclaration()
	{
		for (uint i = 0; i < m_Uniforms.size(); i++)
			gedel (GLShaderUniformDeclaration*)m_Uniforms[i];
	}

	void GLShaderUniformBufferDeclaration::PushUniform(GLShaderUniformDeclaration* uniform)
	{
		uint offset = 0;

		if (m_Uniforms.size())
		{
			GLShaderUniformDeclaration* previous = (GLShaderUniformDeclaration*)m_Uniforms.back();
			offset = previous->m_Offset + previous->m_Size;
		}

		uniform->setOffset(offset);
		m_Size += uniform->getSize();
		m_Uniforms.push_back(uniform);
	}

	ShaderUniformDeclaration* GLShaderUniformBufferDeclaration::FindUniform(const String& name)
	{
		for (ShaderUniformDeclaration* uniform : m_Uniforms)
		{
			if (uniform->getName() == name)
				return uniform;
		}

		return nullptr;
	}

} }