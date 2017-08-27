#pragma once

#include "CustomString.h"
#include "Backend/API/APIShader.h"

#include "DX11Common.h"
#include "DX11ShaderResource.h"
#include "DX11ShaderUniform.h"

namespace gebase { namespace graphics { namespace API {

	struct DX11ShaderErrorInfo
	{
		String profile;
		String message;
	};

	class DX11Shader : public APIShader
	{
	private:
		friend class APIShader;
		friend class Shader;
		friend class ShaderManager;
		
		struct Data
		{
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader; //Fragment shader
			ID3DBlob* vs;
			ID3DBlob* ps;
		};

		static const DX11Shader* s_CurrentlyBound;

		String m_Name;
		String m_Filepath;
		mutable Data m_Data;

		ShaderUniformBufferList m_VSUniformBuffers;
		ShaderUniformBufferList m_FSUniformBuffers;
		DX11ShaderUniformBufferDeclaration* m_VSUserUniformBuffer;
		DX11ShaderUniformBufferDeclaration* m_FSUserUniformBuffer;
		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;

		ID3D11Buffer** m_VSConstantBuffers;
		uint m_VSConstantBuffersCount;

		ID3D11Buffer** m_FSConstantBuffers;
		uint m_FSConstantBuffersCount;

		static ID3DBlob* Compile(const String& source, const String& profile, const String& main, DX11ShaderErrorInfo& info);
		void Load(const String& source);

		String RemoveComments(const String& source);
		
		void Parse(const String& source);
		void ParseCBuffer(const String& block);
		void ParseTexture(const String& statement);
		void ParseSamplerState(const String& statement);
		void ParseStruct(const String& block);

		void CreateBuffers();
		ShaderStruct* FindStruct(const String& name);
	public:
		DX11Shader(const String& name, const String& source);
		~DX11Shader();

		inline Data& getData() const { return m_Data; }

		void Bind() const;
		void Unbind() const;

		void setVSSystemUniformBuffer(byte* data, uint size, uint slot = 0) override;
		void setFSSystemUniformBuffer(byte* data, uint size, uint slot = 0) override;

		void setVSUserUniformBuffer(byte* data, uint size) override;
		void setFSUserUniformBuffer(byte* data, uint size) override;

		inline const ShaderUniformBufferList& getVSSystemUniforms() const override { return m_VSUniformBuffers; }
		inline const ShaderUniformBufferList& getFSSystemUniforms() const override { return m_FSUniformBuffers; }

		inline const ShaderUniformBufferDeclaration* getVSUserUniformBuffer() const override { return m_VSUserUniformBuffer; }
		inline const ShaderUniformBufferDeclaration* getFSUserUniformBuffer() const override { return m_FSUserUniformBuffer; }

		inline const ShaderResourceList& getResources() const override { return m_Resources; }

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Filepath; }

		static bool TryCompile(const String& source, String& error);
		static const DX11Shader* CurrentlyBound() { return s_CurrentlyBound; }
	};

} } }