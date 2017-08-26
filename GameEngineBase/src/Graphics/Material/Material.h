#pragma once

#include "Common.h"
#include "Types.h"

#include "Graphics/Texture/Texture2D.h"
#include "Graphics/Texture/TextureCube.h"
#include "Graphics/Texture/TextureDepth.h"

#include "Graphics/Shader/Shader.h"

namespace gebase { namespace graphics {

	class GE_API Material
	{
	public:
		enum class RenderFlags
		{
			NONE				= 0,
			DISABLE_DEPTH_TEST	= BIT(0),
			WIREFRAME			= BIT(1)
		};
	private:
		friend class MaterialInstance;
	protected:
		Shader* m_Shader;

		byte* m_VSUserUniformBuffer;
		uint m_VSUserUniformBufferSize;

		byte* m_FSUserUniformBuffer;
		uint m_FSUserUniformBufferSize;

		std::vector<Texture*> m_Textures;

		const API::ShaderUniformList* m_VSUserUniforms;
		const API::ShaderUniformList* m_FSUserUniforms;
		const API::ShaderResourceList* m_Resources;

		int m_RenderFlags;
		bool m_isInstance;

		void AllocateStorage();
		API::ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		API::ShaderResourceDeclaration* FindResourceDeclaration(const String& name);
	public:
		Material(Shader* shader);
		~Material();

		void Bind();
		void Unbind();

		void setUniformData(const String& uniform, byte* data);
		void setTexture(const String& name, Texture* texture);

		inline void setRenderFlags(int flags) { m_RenderFlags = flags; }
		inline void setRenderFlag(RenderFlags flag) { m_RenderFlags |= (int)flag; }

		inline int getRenderFlags() const { return m_RenderFlags; }
		inline Shader* getShader() { return m_Shader; }

		template<typename T>
		void setUniform(const String& name, const T& data)
		{
			byte* buffer;
			API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);

			if (!declaration)
			{
				std::cout << "[Material] setUniform() - Declaration is null." << std::endl;
				return;
			}

			memcpy(buffer + declaration->getOffset(), &data, declaration->getSize());
		}

		template<typename T>
		const T* getUniform(const String& name) const
		{
			return getUniform<T>(getUniformDeclaration(name));
		}

		template<typename T>
		const T* getUniform(const API::ShaderUniformDeclaration* unfiorm) const
		{
			return (T*)&m_UniformData[uniform->getOffset()];
		}
	};

	class GE_API MaterialInstance
	{
	private:
		Material* m_Material;

		byte* m_VSUserUniformBuffer;
		uint m_VSUserUniformBufferSize;

		byte* m_FSUserUniformBuffer;
		uint m_FSUserUniformBufferSize;

		std::vector<Texture*> m_Textures;

		const API::ShaderUniformList* m_VSUserUniforms;
		const API::ShaderUniformList* m_FSUserUniforms;
		const API::ShaderResourceList* m_Resources;

		int m_RenderFlags;

		void AllocateStorage();
		API::ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		API::ShaderResourceDeclaration* FindResourceDeclaration(const String& name);
	public:
		MaterialInstance(Material* material);

		inline Material* getMaterial() const { return m_Material; }

		void Bind();
		void Unbind();

		void setUniformData(const String& uniform, byte* data);
		void setTexture(const String& name, Texture* texture);

		inline void setRenderFlags(int flags) { m_RenderFlags = flags; }
		inline void setRenderFlag(Material::RenderFlags flag) { m_RenderFlags |= (int)flag; }

		inline int getRenderFlags() const { return m_RenderFlags; }

		template<typename T>
		void setUniform(const String& name, const T& data)
		{
			byte* buffer;
			API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);

			if (!declaration)
			{
				std::cout << "[Material] setUniform() - Declaration is null." << std::endl;
				return;
			}

			memcpy(buffer + declaration->getOffset(), &data, declaration->getSize());
		}

		template<typename T>
		const T* getUniform(const String& name) const
		{
			return getUniform<T>(getUniformDeclaration(name));
		}
	};
	
} }