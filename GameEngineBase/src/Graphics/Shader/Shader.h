#pragma once

#include "ge.h"
#include "Common.h"

#include "Math/Maths.h"
#include "System/VirtualFileSystem.h"

#include "Backend/API/APIShaderUniform.h"
#include "Backend/API/APIShaderResource.h"
#include "Backend/API/APIShader.h"
#include "Backend/API/APIShaderDeclaration.h"

namespace gebase { namespace graphics {

	class GE_API Shader
	{
	private:
		API::APIShader* m_Instance;
		API::ShaderDeclaration m_Declaration;
		Shader() { }
	public:
		inline void Bind() const { m_Instance->Bind(); }
		inline void Unbind() const { m_Instance->Unbind(); }

		inline void setVSSystemUniformBuffer(byte* data, uint size, uint slot = 0) { m_Instance->setVSSystemUniformBuffer(data, size, slot); }
		inline void setFSSystemUniformBuffer(byte* data, uint size, uint slot = 0) { m_Instance->setFSSystemUniformBuffer(data, size, slot); }

		inline void setVSUserUniformBuffer(byte* data, uint size) { m_Instance->setVSUserUniformBuffer(data, size); }
		inline void setFSUserUniformBuffer(byte* data, uint size) { m_Instance->setFSUserUniformBuffer(data, size); }

		inline const String& getName() const { return m_Instance->getName(); }
		inline const String& getFilepath() const { return m_Instance->getFilepath(); }

		inline const API::ShaderResourceList& getResources() { return m_Instance->getResources(); }

		inline const API::ShaderUniformBufferList& getVSSystemUniforms() { return m_Instance->getVSSystemUniforms(); }
		inline const API::ShaderUniformBufferList& getFSSystemUniforms() { return m_Instance->getFSSystemUniforms(); }

		inline const API::ShaderUniformBufferDeclaration* getVSUserUniformBuffer() { return m_Instance->getVSUserUniformBuffer(); }
		inline const API::ShaderUniformBufferDeclaration* getFSUserUniformBuffer() { return m_Instance->getFSUserUniformBuffer(); }

		inline static bool TryCompile(const String& source, String& error) { return API::APIShader::TryCompile(source, error); }
		inline static bool TryCompileFromFile(const API::ShaderDeclaration& shader, String& error) { return API::APIShader::TryCompileFromFile(shader, error); }

		static Shader* CreateFromFile(const API::ShaderDeclaration& shader, void* address = nullptr);
		static Shader* CreateFromSource(const API::ShaderDeclaration& shader, const String& source);
	};

} }