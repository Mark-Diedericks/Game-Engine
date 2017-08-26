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

		void Submit(Renderer2D* renderer) const override;
		void ValidateFont(const String& name, int32 size = -1);
		void setAlignment(Alignment alignment);
		void setText(const String& text);

		inline const Font& getFont() const { return *m_Font; }
		inline Alignment getAlignment() const { return m_Alignment; }
		inline const String& getText() const { return m_Text; }
	};

} }