#include "ge.h"
#include "Material.h"

#include "Graphics/Texture/Texture2D.h"
#include "System/Memory.h"
#include "Backend/API/APIShaderResource.h"

#include <sstream>

namespace gebase { namespace graphics {

	Material::Material(Shader* shader) : IRenderAPIDependant(RenderObjectType::Material), m_Shader(shader), m_isInstance(false)
	{
		AllocateStorage();
		m_Resources = &shader->getResources();
	}

	Material::~Material()
	{
		gedel[] m_VSUserUniformBuffer;
		gedel[] m_FSUserUniformBuffer;
	}

	bool Material::EmployRenderAPI(RenderAPI api)
	{
		if (!m_Shader->EmployRenderAPI(api))
			return false;

		for (uint i = 0; i < m_Textures.size(); i++)
			if(!m_Textures[i]->EmployRenderAPI(api))
				return false;

		AllocateStorage();
		m_Resources = &m_Shader->getResources();

		return true;
	}

	void Material::AllocateStorage()
	{
		m_VSUserUniformBuffer = nullptr;
		m_VSUserUniformBufferSize = 0;

		m_FSUserUniformBuffer = nullptr;
		m_FSUserUniformBufferSize = 0;

		m_VSUserUniforms = nullptr;
		m_FSUserUniforms = nullptr;
		
		const API::ShaderUniformBufferDeclaration* vsBuffer = m_Shader->getVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->getSize();
			m_VSUserUniformBuffer = genew byte[m_VSUserUniformBufferSize];
			memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
			m_VSUserUniforms = &vsBuffer->getUniformDeclarations();
		}

		const API::ShaderUniformBufferDeclaration* fsBuffer = m_Shader->getFSUserUniformBuffer();
		if (fsBuffer)
		{
			m_FSUserUniformBufferSize = fsBuffer->getSize();
			m_FSUserUniformBuffer = genew byte[m_FSUserUniformBufferSize];
			memset(m_FSUserUniformBuffer, 0, m_FSUserUniformBufferSize);
			m_FSUserUniforms = &fsBuffer->getUniformDeclarations();
		}
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (!m_isInstance)
		{
			if (m_VSUserUniformBuffer)
				m_Shader->setVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);

			if (m_FSUserUniformBuffer)
				m_Shader->setFSUserUniformBuffer(m_FSUserUniformBuffer, m_FSUserUniformBufferSize);
		}

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	void Material::Unbind()
	{
		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Unbind(i);
		}
	}

	void Material::setUniformData(const String& uniform, byte* data)
	{
		byte* buffer;
		API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);

		if (!buffer)
		{
			std::cout << "[Material] setUniformData() - Buffer is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		memcpy(buffer + declaration->getOffset(), data, declaration->getSize());
	}

	void Material::setTexture(const String& name, Texture* texture)
	{
		API::ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			std::cout << "[Material] setTexture() - Resource declaration is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		uint slot = declaration->getRegister();

		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);

		m_Textures[slot] = texture;
	}

	API::ShaderUniformDeclaration* Material::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (API::ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
			{
				if (uniform->getName() == name)
				{
					*outBuffer = m_VSUserUniformBuffer;
					return uniform;
				}
			}
		}

		if (m_FSUserUniforms)
		{
			for (API::ShaderUniformDeclaration* uniform : *m_FSUserUniforms)
			{
				if (uniform->getName() == name)
				{
					*outBuffer = m_FSUserUniformBuffer;
					return uniform;
				}
			}
		}

		return nullptr;
	}

	API::ShaderResourceDeclaration* Material::FindResourceDeclaration(const String& name)
	{
		for (API::ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->getName() == name)
				return resource;
		}

		return nullptr;
	}

	//MATERIAL INSTANCE START
	MaterialInstance::MaterialInstance(Material* material) : IRenderAPIDependant(RenderObjectType::Material), m_Material(material)
	{
		AllocateStorage();

		memcpy(m_VSUserUniformBuffer, m_Material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		memcpy(m_FSUserUniformBuffer, m_Material->m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		m_Resources = &m_Material->getShader()->getResources();
		m_RenderFlags = material->m_RenderFlags;
		m_Material->m_isInstance = true;
	}

	bool MaterialInstance::EmployRenderAPI(RenderAPI api)
	{
		if (!m_Material->EmployRenderAPI(api))
			return false;

		for (uint i = 0; i < m_Textures.size(); i++)
			if (!m_Textures[i]->EmployRenderAPI(api))
				return false;

		AllocateStorage();

		memcpy(m_VSUserUniformBuffer, m_Material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		memcpy(m_FSUserUniformBuffer, m_Material->m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		m_Resources = &m_Material->getShader()->getResources();
		m_RenderFlags = m_Material->m_RenderFlags;
		m_Material->m_isInstance = true;

		return true;
	}

	void MaterialInstance::AllocateStorage()
	{
		const API::ShaderUniformBufferDeclaration* vsBuffer = m_Material->m_Shader->getVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->getSize();
			m_VSUserUniformBuffer = genew byte[m_VSUserUniformBufferSize];
			memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
			m_VSUserUniforms = &vsBuffer->getUniformDeclarations();
		}

		const API::ShaderUniformBufferDeclaration* fsBuffer = m_Material->m_Shader->getFSUserUniformBuffer();
		if (fsBuffer)
		{
			m_FSUserUniformBufferSize = fsBuffer->getSize();
			m_FSUserUniformBuffer = genew byte[m_FSUserUniformBufferSize];
			memset(m_FSUserUniformBuffer, 0, m_FSUserUniformBufferSize);
			m_FSUserUniforms = &fsBuffer->getUniformDeclarations();
		}
	}

	void MaterialInstance::Bind()
	{
		m_Material->Bind();
		if (m_VSUserUniformBuffer)
			m_Material->m_Shader->setVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);

		if (m_FSUserUniformBuffer)
			m_Material->m_Shader->setFSUserUniformBuffer(m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	void MaterialInstance::Unbind()
	{
		m_Material->Unbind();

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Unbind(i);
		}
	}

	void MaterialInstance::setUniformData(const String& uniform, byte* data)
	{
		byte* buffer;
		API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);

		if (!buffer)
		{
			std::cout << "[MaterialInstance] setUniformData() - Buffer is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		memcpy(buffer + declaration->getOffset(), data, declaration->getSize());
	}
	
	void MaterialInstance::setTexture(const String& name, Texture* texture)
	{
		API::ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			std::cout << "[Material] setTexture() - Resource declaration is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		uint slot = declaration->getRegister();

		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);

		m_Textures[slot] = texture;
	}

	API::ShaderUniformDeclaration* MaterialInstance::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (API::ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
			{
				if (uniform->getName() == name)
				{
					*outBuffer = m_VSUserUniformBuffer;
					return uniform;
				}
			}
		}

		if (m_FSUserUniforms)
		{
			for (API::ShaderUniformDeclaration* uniform : *m_FSUserUniforms)
			{
				if (uniform->getName() == name)
				{
					*outBuffer = m_FSUserUniformBuffer;
					return uniform;
				}
			}
		}

		return nullptr;
	}

	API::ShaderResourceDeclaration* MaterialInstance::FindResourceDeclaration(const String& name)
	{
		for (API::ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->getName() == name)
				return resource;
		}

		return nullptr;
	}

} }