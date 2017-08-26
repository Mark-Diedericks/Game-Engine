#include "Pch.h"
#include "Label.h"
#include "Renderer/Renderer2D.h"

namespace gebase { namespace graphics {

	Label::Label(const String& text, float x, float y, uint color, Alignment alignment) : Sprite(), m_Font(FontManager::Get("SourceSansPro"))
	{
		setPosition(math::Vector3f(x, y, 0.0f));
		setText(text);
		setAlignment(alignment);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, Font* font, uint color, Alignment alignment) : Sprite(), m_Font(font)
	{
		setPosition(math::Vector3f(x, y, 0.0f));
		setText(text);
		setAlignment(alignment);
		m_Color = color;
	}

	Label::Label(const String& text, float x, float y, const String& font, uint color, Alignment alignment) : Sprite(), m_Font(FontManager::Get(font))
	{
		setPosition(math::Vector3f(x, y, 0.0f));
		setText(text);
		setAlignment(alignment);
		m_Color = color;
		ValidateFont(font);
	}

	Label::Label(const String& text, float x, float y, const String& font, uint size, uint color, Alignment alignment) : Sprite(), m_Font(FontManager::Get(font, size))
	{
		setPosition(math::Vector3f(x, y, 0.0f));
		setText(text);
		setAlignment(alignment);
		m_Color = color;
		ValidateFont(font, size);
	}

	void Label::Submit(Renderer2D* renderer) const
	{
		renderer->DrawString(m_Text, m_Bounds.getMinimumBound() + m_AlignmentOffset, *m_Font, m_Color);
	}

	void Label::UpdateBounds()
	{
		math::Vector2f size = m_Font->getSize(m_Text) * 0.5f;
		m_Bounds.size = size;

		switch (m_Alignment)
		{
		case Alignment::LEFT:
			m_AlignmentOffset.x = size.x;
			break;
		case Alignment::CENTER:
			m_AlignmentOffset.x = 0.0f;
			break;
		case Alignment::RIGHT:
			m_AlignmentOffset.x = -size.x;
			break;
		}
	}

	void Label::ValidateFont(const String& name, int32 size)
	{
		if (m_Font != nullptr)
			return;

		std::cout << "[Label] ValidateFont() - m_Font is null.";

		if (size > 0)
			std::cout << "[Label] ValidateFont() - Font size is " << size;

		std::cout << std::endl;

		m_Font = FontManager::Get("SourceSansPro");
	}

	void Label::setAlignment(Alignment alignment)
	{
		m_Alignment = alignment;
		UpdateBounds();
	}

	void Label::setText(const String& text)
	{
		m_Text = text;
		UpdateBounds();
	}

} }