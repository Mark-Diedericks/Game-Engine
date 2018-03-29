#pragma once

#include "Common.h"
#include "Types.h"
#include "Font.h"
#include "Graphics/Renderer/IRenderer.h"

namespace gebase { namespace graphics {

	class GE_API FontManager : public IRenderer
	{
	private:
		static std::vector<Font*> s_Fonts;
		static math::Vector2f s_Scale;

		FontManager() { }
	public:
		static void setScale(const math::Vector2f& scale);
		static inline const math::Vector2f& getScale() { return s_Scale; }

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;

		static void Clean();
		static void Add(Font* font);

		static Font* Get();
		static Font* Get(const String& name);
		static Font* Get(uint size);
		static Font* Get(const String& name, uint size);
	};

} }