#pragma once

#include "Common.h"
#include "Types.h"

#include "Utils\LogUtil.h"

#include "Graphics/Texture/Texture2D.h"
#include "Graphics/Texture/TextureCube.h"
#include "Graphics/Texture/TextureDepth.h"

#include "Graphics/Shader/Shader.h"

namespace gebase { namespace graphics {

	class GE_API Material : public IRenderAPIDependantContainer
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
		std::list<Texture*> m_TempTextures;

		const ShaderUniformList* m_VSUserUniforms;
		const ShaderUniformList* m_FSUserUniforms;
		const ShaderResourceList* m_Resources;

		int m_RenderFlags;
		bool m_isInstance;

		void AllocateStorage();
		ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		ShaderResourceDeclaration* FindResourceDeclaration(const String& name);

		bool m_DoneShader = false;
		bool m_DoneTexture2D = false;
		bool m_DoneTextureCube = false;
		bool m_DoneTextureDepth = false;
	public:
		Material(Shader* shader);
		~Material();

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;

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
			ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);

			if (!declaration)
			{
				utils::LogUtil::WriteLine("ERROR", "[Material] setUniform() - Declaration is null, " + name);
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
		const T* getUniform(const ShaderUniformDeclaration* unfiorm) const
		{
			return (T*)&m_UniformData[uniform->getOffset()];
		}
	};

	class GE_API MaterialInstance : public IRenderAPIDependantContainer
	{
	private:
		Material* m_Material;

		byte* m_VSUserUniformBuffer;
		uint m_VSUserUniformBufferSize;

		byte* m_FSUserUniformBuffer;
		uint m_FSUserUniformBufferSize;

		std::vector<Texture*> m_Textures;
		std::list<Texture*> m_TempTextures;

		const ShaderUniformList* m_VSUserUniforms;
		const ShaderUniformList* m_FSUserUniforms;
		const ShaderResourceList* m_Resources;

		int m_RenderFlags;

		void AllocateStorage();
		ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		ShaderResourceDeclaration* FindResourceDeclaration(const String& name);

		bool m_DoneShader = false;
		bool m_DoneTexture2D = false;
		bool m_DoneTextureCube = false;
		bool m_DoneTextureDepth = false;
		bool m_DoneFramebuffer2D = false;
		bool m_DoneFramebufferDepth = false;
		bool m_DoneIndexBuffer = false;

		uint m_TexturesSize;
		uint m_MaterialTexturesSize;
		uint m_MAX_SIZE;
	public:
		MaterialInstance(Material* material);

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;

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
			ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);

			if (!declaration)
			{
				utils::LogUtil::WriteLine("ERROR", "[Material] setUniform() - Declaration is null, " + name);
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