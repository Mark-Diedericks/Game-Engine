#pragma once

#include "Common.h"
#include "Sprite.h"
#include "Font/FontManager.h"

namespace gebase { namespace graphics {

	class GE_API Label : public Sprite
	{
	public:
		enum class Alignment
		{
			NONE = 0,
			LEFT,
			CENTER,
			RIGHT
		};
	private:
		String m_Text;
		Font* m_Font;
		Alignment m_Alignment;
		math::Vector2f m_AlignmentOffset;

		void UpdateBounds();
	public:
		Label(const String& text, float x, float y, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, Font* font, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, const String& font, uint color, Alignment alignment = Alignment::LEFT);
		Label(const String& text, float x, float y, const String& font, uint size, uint color, Alignment alignment = Alignment::LEFT);


		inline bool EmployRenderAPIShader(RenderAPI api) override { return Sprite::EmployRenderAPIShader(api); }
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		inline bool EmployRenderAPITextureCube(RenderAPI api) override { return Sprite::EmployRenderAPITextureCube(api); }
		inline bool EmployRenderAPITextureDepth(RenderAPI api) override { return Sprite::EmployRenderAPITextureDepth(api); }
		inline bool EmployRenderAPIFramebuffer2D(RenderAPI api) override { return Sprite::EmployRenderAPIFramebuffer2D(api); }
		inline bool EmployRenderAPIFramebufferDepth(RenderAPI api) override { return Sprite::EmployRenderAPIFramebufferDepth(api); }
		inline bool EmployRenderAPIIndexBuffer(RenderAPI api) override { return Sprite::EmployRenderAPIIndexBuffer(api); }
		inline bool EmployRenderAPIVertexBuffer(RenderAPI api) override { return Sprite::EmployRenderAPIVertexBuffer(api); }
		inline bool EmployRenderAPIVertexArray(RenderAPI api) override { return Sprite::EmployRenderAPIVertexArray(api); }

		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const String& name, int32 size = -1);
		void setAlignment(Alignment alignment);
		void setText(const String& text);

		inline const Font& getFont() const { return *m_Font; }
		inline Alignment getAlignment() const { return m_Alignment; }
		inline const String& getText() const { return m_Text; }
	};

} }