#pragma once

#include "CustomString.h"
#include "Graphics/Shader/Shader.h"

#include "DX11Common.h"
#include "DX11ShaderResource.h"
#include "DX11ShaderUniform.h"

namespace gebase { namespace graphics {

	struct DX11ShaderErrorInfo
	{
		String profile;
		String message;
	};

	class DX11Shader : public Shader
	{
	private:
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

		const ShaderDeclaration m_Declaration;
		const ShaderSource m_Source;
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
		DX11Shader(const ShaderDeclaration& declaration, const ShaderSource& source);
		~DX11Shader();

		inline Data& getData() const { return m_Data; }
		
		inline ShaderDeclaration getDeclaration() const override { return m_Declaration; }
		inline ShaderSource getSource() const override { return m_Source; }

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

		inline const String& getName() const override { return m_Declaration.name; }
		inline const String& getFilepath() const override { return m_Declaration.d3d11; }

		static bool TryCompile(const String& source, String& error);
		static const DX11Shader* CurrentlyBound() { return s_CurrentlyBound; }
	};

} }