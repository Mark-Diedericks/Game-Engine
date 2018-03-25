#include "ge.h"
#include "Common.h"
#include "CustomString.h"

#include "DX11Shader.h"
#include "DX11Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { 

	const DX11Shader* DX11Shader::s_CurrentlyBound = nullptr;

	bool DX11Shader::TryCompile(const String& source, String& error)
	{
		DX11ShaderErrorInfo info;

		if (!Compile(source, "vs_4_0", "VSMain", info))
		{
			error += info.message;
			return false;
		}

		if (!Compile(source, "ps_4_0", "PSMain", info))
		{
			error += info.message;
			return false;
		}

		return true;
	}

	DX11Shader::DX11Shader(const ShaderDeclaration& declaration, const ShaderSource& source) : Shader(0), m_Declaration(declaration), m_Source(source)
	{
		m_VSUserUniformBuffer = nullptr;
		m_FSUserUniformBuffer = nullptr;

		Load(source.d3d11);
		Parse(source.d3d11);
		CreateBuffers();
	}

	DX11Shader::~DX11Shader()
	{
		m_Data.vertexShader->Release();
		m_Data.pixelShader->Release();

		gedel m_VSUserUniformBuffer;
		gedel m_FSUserUniformBuffer;

		for (uint i = 0; i < m_VSConstantBuffersCount; i++)
			gedel m_VSConstantBuffers[i];

		gedel[] m_VSConstantBuffers;

		for (uint i = 0; i < m_FSConstantBuffersCount; i++)
			gedel m_FSConstantBuffers[i];

		gedel[] m_FSConstantBuffers;
	}
	
	String DX11Shader::RemoveComments(const String& source)
	{
		const char* str = source.c_str();

		String result = source;
		int32 startPos;
		while ((startPos = IndexOf(result, "/*")) != -1)
		{
			int32 endPos = IndexOf(result, "*/");

			if(endPos == -1)
			{
				utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] RemoveComments() - End position (*/) == -1.");
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			result = Remove(result, startPos, endPos - startPos + 2);
		}

		while ((startPos = IndexOf(result, "//")) != -1)
		{
			int32 endPos = IndexOf(result, "\n", startPos);

			if (endPos == -1)
			{
				utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] RemoveComments() - End position (\n) == -1.");
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			result = Remove(result, startPos, endPos - startPos + 1);
		}

		return result;
	}

	void DX11Shader::Load(const String& source)
	{
		DX11ShaderErrorInfo info;

		m_Data.vs = Compile(source, "vs_4_0", "VSMain", info);
		if (!m_Data.vs)
		{
			utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] Load() - m_Data.vs assertion failed");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		m_Data.ps = Compile(source, "ps_4_0", "PSMain", info);
		if (!m_Data.ps)
		{
			utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] Load() - m_Data.ps assertion failed");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		DX11Context::getDevice()->CreateVertexShader(m_Data.vs->GetBufferPointer(), m_Data.vs->GetBufferSize(), NULL, &m_Data.vertexShader);
		DX11Context::getDevice()->CreatePixelShader(m_Data.ps->GetBufferPointer(), m_Data.ps->GetBufferSize(), NULL, &m_Data.pixelShader);
	}

	ID3DBlob* DX11Shader::Compile(const String& source, const String& profile, const String& main, DX11ShaderErrorInfo& info)
	{
		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT status = D3DCompile(source.c_str(), source.size(), NULL, NULL, NULL, main.c_str(), profile.c_str(), D3DCOMPILE_DEBUG, 0, &shaderBlob, &errorBlob);

		if (status != S_OK)
			info.message += "Unable to compile shader from source.\n";

		if (errorBlob)
		{
			info.profile += profile + "\n";

			if (errorBlob->GetBufferSize())
			{
				utils::LogUtil::WriteLine("ERROR", "[DX11Shader] Compile() - Shader compile errors;");
				utils::LogUtil::WriteLine("ERROR", "	" + (String)(char*)errorBlob->GetBufferPointer());
			}

			errorBlob->Release();
		}

		if (status == S_OK)
			return shaderBlob;

		return nullptr;
	}

	void DX11Shader::Parse(const String& source)
	{
		const char* token;
		const char* str;

		String src = RemoveComments(source);

		str = src.c_str();
		while (token = FindToken(str, "struct"))
			ParseStruct(GetBlock(token, &str));

		str = src.c_str();
		while (token = FindToken(str, "cbuffer"))
			ParseCBuffer(GetBlock(token, &str));

		str = src.c_str();
		while (token = FindToken(str, "Texture2D"))
			ParseTexture(GetStatement(token, &str));

		str = src.c_str();
		while (token = FindToken(str, "TextureCube"))
			ParseTexture(GetStatement(token, &str));

		str = src.c_str();
		while (token = FindToken(str, "SamplerState"))
			ParseSamplerState(GetStatement(token, &str));
	}

	void DX11Shader::ParseStruct(const String& block)
	{
		std::vector<String> tokens = Tokenize(block);
		uint index = 0;
		index++; //skip "struct"
		
		String structName = tokens[index++];
		ShaderStruct* shaderStruct = genew ShaderStruct(structName);

		index++; // skip "{"
		
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			String type = tokens[index++];
			String name = tokens[index++];

			if (type == ":") //TODO, fix
				continue;

			if (const char* s = strstr(name.c_str(), ";"))
				name = String(name.c_str(), s - name.c_str());

			ShaderUniformDeclaration* field = genew DX11ShaderUniformDeclaration(DX11ShaderUniformDeclaration::StringToType(type), name);
			shaderStruct->AddField(field);
		}

		m_Structs.push_back(shaderStruct);
	}

	void DX11Shader::ParseCBuffer(const String& block)
	{
		std::vector<String> tokens = Tokenize(block);
		uint index = 1;
		
		String bufferName = tokens[index++];
		uint reg = 0;

		if (tokens[index++] == ":")
		{
			String cbRegister = tokens[index++];
			reg = NextInt(cbRegister);
		}

		DX11ShaderUniformBufferDeclaration* buffer = nullptr;

		uint shaderType;
		if (StartsWith(bufferName, "VS"))
			shaderType = 0;
		else if (StartsWith(bufferName, "PS"))
			shaderType = 1;
		else
		{
			utils::LogUtil::WriteLine("WARNING", "[DX11Shader: " + getName() + "] ParseCBuffer() - ShaderType is unknown.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		index++;
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			String type = tokens[index++];
			String name = tokens[index++];

			if (const char* s = strstr(name.c_str(), ";"))
				name = String(name.c_str(), s - name.c_str());

			if (buffer == nullptr)
			{
				buffer = new DX11ShaderUniformBufferDeclaration(bufferName, reg, shaderType);

				if (StartsWith(name, "sys_"))
				{
					switch (shaderType)
					{
					case 0:
						m_VSUniformBuffers.push_back(buffer);
						break;
					case 1:
						m_FSUniformBuffers.push_back(buffer);
						break;
					}
				}
				else
				{
					switch (shaderType)
					{
					case 0:
						if (m_VSUserUniformBuffer != nullptr)
						{
							utils::LogUtil::WriteLine("WARNING", "[DX11Shader: " + getName() + "] ParseCBuffer - m_VSUserUniformBuffer != nullptr");
#ifdef GE_DEBUG
							__debugbreak();
#endif
						}
						m_VSUserUniformBuffer = buffer;
						break;
					case 1:
						if (m_FSUserUniformBuffer != nullptr)
						{
							utils::LogUtil::WriteLine("WARNING", "[DX11Shader: " + getName() + "] ParseCBuffer - m_FSUserUniformBuffer != nullptr");
#ifdef GE_DEBUG
							__debugbreak();
#endif
						}
						m_FSUserUniformBuffer = buffer;
						break;
					}
				}
			}

			DX11ShaderUniformDeclaration::UniformType t = DX11ShaderUniformDeclaration::StringToType(type);
			DX11ShaderUniformDeclaration* declaration = nullptr;

			if (t == DX11ShaderUniformDeclaration::UniformType::NONE)
			{
				ShaderStruct* s = FindStruct(type);

				if (!s)
				{
					utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] ParseCBuffer() - Could not find struct; " + type + "	" + name);
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				declaration = new DX11ShaderUniformDeclaration(s, name);
			}
			else
			{
				declaration = new DX11ShaderUniformDeclaration(t, name);
			}

			buffer->PushUniform(declaration);
		}

		buffer->Align();
	}


	void DX11Shader::ParseTexture(const String& statement)
	{
		std::vector<String> tokens = Tokenize(statement);
		uint index = 0;
		uint reg = 0;

		String type = tokens[index++];
		String name = tokens[index++];

		if (tokens[index++] == ":")
		{
			String texRegister = tokens[index++];
			reg = NextInt(texRegister);
		}
		
		DX11ShaderResourceDeclaration* declaration = genew DX11ShaderResourceDeclaration(DX11ShaderResourceDeclaration::StringToType(type), name);
		declaration->m_Register = reg;
		m_Resources.push_back(declaration);
	}

	void DX11Shader::ParseSamplerState(const String& statement)
	{
		std::vector<String> tokens = Tokenize(statement);

		if (tokens.front() != "SamplerState")
		{
			utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] ParseSamplerState() - Statement is not a SamplerState.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		uint reg = 0;
		uint index = 1;

		String name = tokens[index++];

		if (tokens[index++] == ":")
		{
			String texRegister = tokens[index++];
			reg = NextInt(texRegister);
		}

		//DX11ShaderResourceDeclaration* declaration = genew DX11ShaderResourceDeclaration(DX11ShaderResourceDeclaration::ResourceType::SAMPLERSTATE, name);
		//declaration->m_Register = reg;
		//m_Resources.push_back(declaration);
	}
	
	void DX11Shader::CreateBuffers()
	{
		//Vertex Shader
		m_VSConstantBuffersCount = m_VSUniformBuffers.size() + (m_VSUserUniformBuffer ? 1 : 0);
		m_VSConstantBuffers = new ID3D11Buffer*[m_VSConstantBuffersCount];

		for (uint i = 0; i < m_VSUniformBuffers.size(); i++)
		{
			DX11ShaderUniformBufferDeclaration* declaration = (DX11ShaderUniformBufferDeclaration*)m_VSUniformBuffers[i];

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = declaration->getSize();
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = new byte[desc.ByteWidth];
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			DX11Context::getDevice()->CreateBuffer(&desc, &sd, &m_VSConstantBuffers[declaration->getRegister()]);
		}

		if (m_VSUserUniformBuffer)
		{
			DX11ShaderUniformBufferDeclaration* declaration = (DX11ShaderUniformBufferDeclaration*)m_VSUserUniformBuffer;

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = declaration->getSize();
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = new byte[desc.ByteWidth];
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			DX11Context::getDevice()->CreateBuffer(&desc, &sd, &m_VSConstantBuffers[declaration->getRegister()]);
		}

		//Pixel Shader
		m_FSConstantBuffersCount = m_FSUniformBuffers.size() + (m_FSUserUniformBuffer ? 1 : 0);
		m_FSConstantBuffers = new ID3D11Buffer*[m_FSConstantBuffersCount];

		for (uint i = 0; i < m_FSUniformBuffers.size(); i++)
		{
			DX11ShaderUniformBufferDeclaration* declaration = (DX11ShaderUniformBufferDeclaration*)m_FSUniformBuffers[i];

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = declaration->getSize();
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = new byte[desc.ByteWidth];
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			DX11Context::getDevice()->CreateBuffer(&desc, &sd, &m_FSConstantBuffers[declaration->getRegister()]);
		}

		if (m_FSUserUniformBuffer)
		{
			DX11ShaderUniformBufferDeclaration* declaration = (DX11ShaderUniformBufferDeclaration*)m_FSUserUniformBuffer;

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = declaration->getSize();
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = new byte[desc.ByteWidth];
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			DX11Context::getDevice()->CreateBuffer(&desc, &sd, &m_FSConstantBuffers[declaration->getRegister()]);
		}
	}

	void DX11Shader::setVSSystemUniformBuffer(byte* data, uint size, uint slot)
	{
		if (m_VSUserUniformBuffer)
			if (slot == m_VSUserUniformBuffer->getRegister())
			{
				utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] setVSSystemUniformBuffer() - Buffer slot is equal to user uniform buffer slot." );
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

		ID3D11Buffer* cbuffer = m_VSConstantBuffers[slot];

		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DX11Context::getDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
		memcpy(msr.pData, data, size);
		DX11Context::getDeviceContext()->Unmap(cbuffer, NULL);
	}

	void DX11Shader::setFSSystemUniformBuffer(byte* data, uint size, uint slot)
	{
		if (m_FSUserUniformBuffer)
			if (slot == m_FSUserUniformBuffer->getRegister())
			{
				utils::LogUtil::WriteLine("ERROR", "[DX11Shader: " + getName() + "] setFSSystemUniformBuffer() - Buffer slot is equal to user uniform buffer slot.");
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

		ID3D11Buffer* cbuffer = m_FSConstantBuffers[slot];

		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DX11Context::getDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
		memcpy(msr.pData, data, size);
		DX11Context::getDeviceContext()->Unmap(cbuffer, NULL);
	}

	void DX11Shader::setVSUserUniformBuffer(byte* data, uint size)
	{
		ID3D11Buffer* cbuffer = m_VSConstantBuffers[m_VSUserUniformBuffer->getRegister()];

		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DX11Context::getDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
		memcpy(msr.pData, data, size);
		DX11Context::getDeviceContext()->Unmap(cbuffer, NULL);
	}

	void DX11Shader::setFSUserUniformBuffer(byte* data, uint size)
	{
		ID3D11Buffer* cbuffer = m_FSConstantBuffers[m_FSUserUniformBuffer->getRegister()];

		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DX11Context::getDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
		memcpy(msr.pData, data, size);
		DX11Context::getDeviceContext()->Unmap(cbuffer, NULL);
	}

	void DX11Shader::Bind() const
	{
		s_CurrentlyBound = this;

		DX11Context::getDeviceContext()->VSSetShader(m_Data.vertexShader, NULL, 0);
		DX11Context::getDeviceContext()->PSSetShader(m_Data.pixelShader, NULL, 0);

		DX11Context::getDeviceContext()->VSSetConstantBuffers(0, m_VSConstantBuffersCount, m_VSConstantBuffers);
		DX11Context::getDeviceContext()->PSSetConstantBuffers(0, m_FSConstantBuffersCount, m_FSConstantBuffers);
	}

	void DX11Shader::Unbind() const
	{

	}

	ShaderStruct* DX11Shader::FindStruct(const String& name)
	{
		for (ShaderStruct* s : m_Structs)
			if (s->getName() == name)
				return s;

		return nullptr;
	}


} }