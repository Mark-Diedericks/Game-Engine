#include "Pch.h"
#include "FontManager.h"
#include "Embedded/Embedded.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	std::vector<Font*> FontManager::s_Fonts;
	math::Vector2f FontManager::s_Scale = math::Vector2f(1.0f, 1.0f);

	void FontManager::setScale(const math::Vector2f& scale)
	{
		s_Scale = scale;
	}

	void FontManager::Clean()
	{
		for (uint i = 0; i < s_Fonts.size(); i++)
			gedel s_Fonts[i];
	}

	void FontManager::Add(Font* font)
	{
		font->setScale(s_Scale);
		s_Fonts.push_back(font);
	}

	Font* FontManager::Get()
	{
		return s_Fonts[0];
	}

	Font* FontManager::Get(const String& name)
	{
		for (Font* font : s_Fonts)
		{
			if (font->getName() == name)
				return font;
		}

		return nullptr;
	}

	Font* FontManager::Get(uint size)
	{
		for (Font* font : s_Fonts)
		{
			if (font->getFontSize() == size)
				return font;
		}

		Font* res = new Font("SourceSansPro", DEFAULT_FONT, DEFAULT_FONT_SIZE, (float)size);
		res->setScale(s_Scale);
		Add(res);
		return res;
	}
	
	Font* FontManager::Get(const String& name, uint size)
	{
		for (Font* font : s_Fonts)
		{
			if (font->getName() == name && font->getFontSize() == size)
				return font;
		}

		return nullptr;
	}

} }