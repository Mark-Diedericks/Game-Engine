#include "ge.h"
#include "Material.h"

#include "Graphics/Texture/Texture2D.h"
#include "System/Memory.h"
#include "Graphics/Shader/ShaderResource.h"
#include "Utils/LogUtil.h"

#include <sstream>

namespace gebase { namespace graphics {

	Material::Material(Shader* shader) : m_Shader(shader), m_isInstance(false)
	{
		AllocateStorage();
		m_Resources = &shader->getResources();
	}

	Material::~Material()
	{
		gedel[] m_VSUserUniformBuffer;
		gedel[] m_FSUserUniformBuffer;
	}

	void Material::AllocateStorage()
	{
		m_VSUserUniformBuffer = nullptr;
		m_VSUserUniformBufferSize = 0;

		m_FSUserUniformBuffer = nullptr;
		m_FSUserUniformBufferSize = 0;

		m_VSUserUniforms = nullptr;
		m_FSUserUniforms = nullptr;
		
		const ShaderUniformBufferDeclaration* vsBuffer = m_Shader->getVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->getSize();
			m_VSUserUniformBuffer = genew byte[m_VSUserUniformBufferSize];
			memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
			m_VSUserUniforms = &vsBuffer->getUniformDeclarations();
		}

		const ShaderUniformBufferDeclaration* fsBuffer = m_Shader->getFSUserUniformBuffer();
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

		if (m_VSUserUniformBuffer)
			m_Shader->setVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);

		if (m_FSUserUniformBuffer)
			m_Shader->setFSUserUniformBuffer(m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

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
		ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);

		if (!buffer)
		{
			utils::LogUtil::WriteLine("ERROR", "[Material] setUniformData() - Buffer is null.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		memcpy(buffer + declaration->getOffset(), data, declaration->getSize());
	}

	void Material::setTexture(const String& name, Texture* texture)
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			utils::LogUtil::WriteLine("ERROR", "[Material] setTexture() - Resource declaration is null.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		uint slot = declaration->getRegister();

		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);

		texture->setResourceName(name);
		m_Textures[slot] = texture;
	}

	ShaderUniformDeclaration* Material::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
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
			for (ShaderUniformDeclaration* uniform : *m_FSUserUniforms)
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

	ShaderResourceDeclaration* Material::FindResourceDeclaration(const String& name)
	{
		for (ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->getName() == name)
				return resource;
		}

		return nullptr;
	}

	bool Material::EmployRenderAPIShader(RenderAPI api)
	{
		if (m_DoneShader)
			return true;

		m_DoneShader = true;

		m_Shader = Shader::ConvertRenderAPI(api, m_Shader);

		ShaderUniformList vs_uniforms;
		ShaderUniformList fs_uniforms;

		byte* vs_buffer;
		byte* fs_buffer;

		if (m_VSUserUniforms != nullptr) vs_uniforms = *m_VSUserUniforms;
		if (m_FSUserUniforms != nullptr) fs_uniforms = *m_FSUserUniforms;

		if (m_VSUserUniforms != nullptr) vs_buffer = m_VSUserUniformBuffer;
		if (m_FSUserUniforms != nullptr) fs_buffer = m_FSUserUniformBuffer;

		AllocateStorage();
		m_Resources = &m_Shader->getResources();

		if (m_VSUserUniforms != nullptr)
			for (ShaderUniformDeclaration* uniform : vs_uniforms)
				setUniformData(uniform->getName(), vs_buffer + uniform->getOffset());

		if (m_FSUserUniforms != nullptr)
			for (ShaderUniformDeclaration* uniform : fs_uniforms)
				setUniformData(uniform->getName(), fs_buffer + uniform->getOffset());

		if (m_VSUserUniforms != nullptr) gedel[] vs_buffer;
		if (m_FSUserUniforms != nullptr) gedel[] fs_buffer;
		
		m_TempTextures = std::list<Texture*>();

		for (uint i = 0; i < m_Textures.size(); i++)
			m_TempTextures.push_back(m_Textures[i]);

		m_Textures.clear();
		m_Textures.shrink_to_fit();

		return true;
	}

	bool Material::EmployRenderAPITexture2D(RenderAPI api)
	{
		if (m_DoneTexture2D)
			return true;

		m_DoneTexture2D = true;

		m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEX2D)
			{
				Texture2D* tex2d  = Texture2D::ConvertRenderAPI(api, (Texture2D*)tex);
				setTexture(tex2d->getResourceName(), tex2d);
				toDelete.push_back(tex);
			}
		}

		for(Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		return true;
	}

	bool Material::EmployRenderAPITextureCube(RenderAPI api)
	{
		if (m_DoneTextureCube)
			return true;

		m_DoneTextureCube = true;

		m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEXCUBE)
			{
				TextureCube* texcube = TextureCube::ConvertRenderAPI(api, (TextureCube*)tex);
				setTexture(texcube->getResourceName(), texcube);
				toDelete.push_back(tex);
			}
		}

		for (Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		return true;
	}

	bool Material::EmployRenderAPITextureDepth(RenderAPI api)
	{
		if (m_DoneTextureDepth)
			return true;

		m_DoneTextureDepth = true;

		m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEXDEPTH)
			{
				TextureDepth* texdepth = TextureDepth::ConvertRenderAPI(api, (TextureDepth*)tex);
				setTexture(texdepth->getResourceName(), texdepth);
				toDelete.push_back(tex);
			}
		}

		for (Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		m_TempTextures.clear();

		return true;
	}

	bool Material::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		return true;
	}

	bool Material::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		return true;
	}

	bool Material::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		return true;
	}

	bool Material::EmployRenderAPIVertexArray(RenderAPI api)
	{
		m_DoneShader = false;
		m_DoneTexture2D = false;
		m_DoneTextureCube = false;
		m_DoneTextureDepth = false;
		return true;
	}


	//MATERIAL INSTANCE START
	MaterialInstance::MaterialInstance(Material* material) : IRenderAPIDependantContainer(), m_Material(material)
	{
		AllocateStorage();

		memcpy(m_VSUserUniformBuffer, m_Material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		memcpy(m_FSUserUniformBuffer, m_Material->m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		m_Resources = &m_Material->getShader()->getResources();
		m_RenderFlags = material->m_RenderFlags;
		m_Material->m_isInstance = true;
	}

	void MaterialInstance::AllocateStorage()
	{
		m_VSUserUniformBuffer = nullptr;
		m_VSUserUniformBufferSize = 0;

		m_FSUserUniformBuffer = nullptr;
		m_FSUserUniformBufferSize = 0;

		m_VSUserUniforms = nullptr;
		m_FSUserUniforms = nullptr;

		const ShaderUniformBufferDeclaration* vsBuffer = m_Material->m_Shader->getVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->getSize();
			m_VSUserUniformBuffer = genew byte[m_VSUserUniformBufferSize];
			memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
			m_VSUserUniforms = &vsBuffer->getUniformDeclarations();
		}

		const ShaderUniformBufferDeclaration* fsBuffer = m_Material->m_Shader->getFSUserUniformBuffer();
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
		//m_Material->Bind();
		m_Material->m_Shader->Bind();

		if (m_VSUserUniformBuffer)
			m_Material->m_Shader->setVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);

		if (m_FSUserUniformBuffer)
			m_Material->m_Shader->setFSUserUniformBuffer(m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		m_TexturesSize = m_Textures.size();
		m_MaterialTexturesSize = m_Material->m_Textures.size();
		m_MAX_SIZE = m_TexturesSize >= m_MaterialTexturesSize ? m_TexturesSize  : m_MaterialTexturesSize;

		for (uint i = 0; i < m_MAX_SIZE; i++)
		{
			if(i < m_TexturesSize)
				if(m_Textures[i])
					m_Textures[i]->Bind(i);

			if (i < m_MaterialTexturesSize)
				if (m_Material->m_Textures[i])
					m_Material->m_Textures[i]->Bind(i);
		}

		/*for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}*/
	}

	void MaterialInstance::Unbind()
	{
		//m_Material->Unbind();

		for (uint i = 0; i < m_MAX_SIZE; i++)
		{
			if (i < m_TexturesSize)
				if (m_Textures[i])
					m_Textures[i]->Unbind(i);

			if (i < m_MaterialTexturesSize)
				if (m_Material->m_Textures[i])
					m_Material->m_Textures[i]->Unbind(i);
		}

		/*for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Unbind(i);
		}*/
	}

	void MaterialInstance::setUniformData(const String& uniform, byte* data)
	{
		byte* buffer;
		ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);

		if (!buffer)
		{
			utils::LogUtil::WriteLine("ERROR", "[MaterialInstance] setUniformData() - Buffer is null.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		memcpy(buffer + declaration->getOffset(), data, declaration->getSize());
	}
	
	void MaterialInstance::setTexture(const String& name, Texture* texture)
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			utils::LogUtil::WriteLine("ERROR", "[Material] setTexture() - Resource declaration is null.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		uint slot = declaration->getRegister();

		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);

		texture->setResourceName(name);
		m_Textures[slot] = texture;
	}

	ShaderUniformDeclaration* MaterialInstance::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
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
			for (ShaderUniformDeclaration* uniform : *m_FSUserUniforms)
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

	ShaderResourceDeclaration* MaterialInstance::FindResourceDeclaration(const String& name)
	{
		for (ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->getName() == name)
				return resource;
		}

		return nullptr;
	}

	bool MaterialInstance::EmployRenderAPIShader(RenderAPI api)
	{
		if (m_DoneShader)
			return true;

		m_DoneShader = true;

		if (!m_Material->EmployRenderAPIShader(api))
			return false;

		ShaderUniformList vs_uniforms;
		ShaderUniformList fs_uniforms;

		byte* vs_buffer;
		byte* fs_buffer;

		if (m_VSUserUniforms != nullptr) vs_uniforms = *m_VSUserUniforms;
		if (m_FSUserUniforms != nullptr) fs_uniforms = *m_FSUserUniforms;

		if (m_VSUserUniforms != nullptr) vs_buffer = m_VSUserUniformBuffer;
		if (m_FSUserUniforms != nullptr) fs_buffer = m_FSUserUniformBuffer;

		AllocateStorage();

		memcpy(m_VSUserUniformBuffer, m_Material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		memcpy(m_FSUserUniformBuffer, m_Material->m_FSUserUniformBuffer, m_FSUserUniformBufferSize);

		m_Resources = &m_Material->getShader()->getResources();

		if (m_VSUserUniforms != nullptr)
			for (ShaderUniformDeclaration* uniform : vs_uniforms)
				setUniformData(uniform->getName(), vs_buffer + uniform->getOffset());

		if (m_FSUserUniforms != nullptr)
			for (ShaderUniformDeclaration* uniform : fs_uniforms)
				setUniformData(uniform->getName(), fs_buffer + uniform->getOffset());

		if (m_VSUserUniforms != nullptr) gedel[] vs_buffer;
		if (m_FSUserUniforms != nullptr) gedel[] fs_buffer;

		m_TempTextures = std::list<Texture*>();

		for (uint i = 0; i < m_Textures.size(); i++)
			m_TempTextures.push_back(m_Textures[i]);

		m_Textures.clear();
		m_Textures.shrink_to_fit();

		return true;
	}

	bool MaterialInstance::EmployRenderAPITexture2D(RenderAPI api)
	{
		if (m_DoneTexture2D)
			return true;

		m_DoneTexture2D = true;

		if (!m_Material->EmployRenderAPITexture2D(api))
			return false;

		m_Material->m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEX2D)
			{
				Texture2D* tex2d = Texture2D::ConvertRenderAPI(api, (Texture2D*)tex);
				setTexture(tex2d->getResourceName(), tex2d);
				toDelete.push_back(tex);
			}
		}

		for (Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		return true;
	}

	bool MaterialInstance::EmployRenderAPITextureCube(RenderAPI api)
	{
		if (m_DoneTextureCube)
			return true;

		m_DoneTextureCube = true;

		if (!m_Material->EmployRenderAPITextureCube(api))
			return false;

		m_Material->m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEXCUBE)
			{
				TextureCube* texcube = TextureCube::ConvertRenderAPI(api, (TextureCube*)tex);
				setTexture(texcube->getResourceName(), texcube);
				toDelete.push_back(tex);
			}
		}

		for (Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		return true;
	}

	bool MaterialInstance::EmployRenderAPITextureDepth(RenderAPI api)
	{
		if (m_DoneTextureDepth)
			return true;

		m_DoneTextureDepth = true;

		if (!m_Material->EmployRenderAPITextureDepth(api))
			return false;

		m_Material->m_Shader->Bind();
		std::list<Texture*> toDelete;

		for (Texture* tex : m_TempTextures)
		{
			if (tex == nullptr)
				continue;

			if (tex->getTextureType() == TextureType::TEXDEPTH)
			{
				TextureDepth* texdepth = TextureDepth::ConvertRenderAPI(api, (TextureDepth*)tex);
				setTexture(texdepth->getResourceName(), texdepth);
				toDelete.push_back(tex);
			}
		}

		for (Texture* tex : toDelete)
			m_TempTextures.remove(tex);

		toDelete.clear();

		m_TempTextures.clear();

		return true;
	}

	bool MaterialInstance::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		if (m_DoneFramebuffer2D)
			return true;

		m_DoneFramebuffer2D = true;

		if (!m_Material->EmployRenderAPIFramebuffer2D(api))
			return false;

		return true;
	}

	bool MaterialInstance::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		if (m_DoneFramebufferDepth)
			return true;

		m_DoneFramebufferDepth = true;

		if (!m_Material->EmployRenderAPIFramebufferDepth(api))
			return false;

		return true;
	}

	bool MaterialInstance::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		if (m_DoneIndexBuffer)
			return true;

		m_DoneIndexBuffer = true;

		if (!m_Material->EmployRenderAPIIndexBuffer(api))
			return false;

		return true;
	}

	bool MaterialInstance::EmployRenderAPIVertexArray(RenderAPI api)
	{
		if (!m_Material->EmployRenderAPIVertexArray(api))
			return false;

		m_DoneShader = false;
		m_DoneTexture2D = false;
		m_DoneTextureCube = false;
		m_DoneTextureDepth = false;
		m_DoneFramebuffer2D = false;
		m_DoneFramebufferDepth = false;
		m_DoneIndexBuffer = false;

		return true;
	}


} }