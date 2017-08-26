#include "Pch.h"
#include "Font.h"

#include <freetype-gl/freetype-gl.h>

namespace gebase { namespace graphics {

	Font::Font(const String& name, const String& filepath, float size) : m_Name(name), m_Filepath(filepath), m_Size(size), m_Scale(math::Vector2f(1.0f, 1.0f)), m_Texture(nullptr)
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, filepath.c_str());

		API::TextureParameters params = { API::TextureFormat::LUMINANCE_ALPHA, API::TextureFilter::LINEAR, API::TextureWrap::CLAMP_TO_EDGE };
		m_Texture = Texture2D::Create(512, 512, params);
		m_Texture->setData(m_FTAtlas->data);

		if (!m_FTFont)
		{
			std::cout << "[Font] Font() - Failed to load font " << filepath.c_str() << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
	}

	Font::Font(const String& name, const byte* data, uint datasize, float size) : m_Name(name), m_Filepath("NULL"), m_Size(size), m_Scale(math::Vector2f(1.0f, 1.0f)), m_Texture(nullptr)
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_memory(m_FTAtlas, size, data, datasize);

		API::TextureParameters params = { API::TextureFormat::LUMINANCE_ALPHA, API::TextureFilter::LINEAR, API::TextureWrap::CLAMP_TO_EDGE };
		m_Texture = Texture2D::Create(512, 512, params);
		m_Texture->setData(m_FTAtlas->data);

		if (!m_FTFont)
		{
			std::cout << "[Font] Font() - Failed to load font from data." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
	}

	Texture2D* Font::getTexture() const
	{
		UpdateAtlas();
		return m_Texture;
	}

	void Font::UpdateAtlas() const
	{
		if (m_FTAtlas->dirty)
		{
			m_Texture->setData(m_FTAtlas->data);
			m_FTAtlas->dirty = (int)false;
		}
	}

	math::Vector2f Font::getOffsets(const String& text) const
	{
		using namespace ftgl;

		if (text.empty())
			return math::Vector2f(0.0f, 0.0f);

		texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[0]);

		if (!glyph)
		{
			std::cout << "[Font] getOffsets() - Glyph is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		float yo = 0.0f;
		const math::Vector2f& scale = m_Scale;

		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* g = texture_font_get_glyph(m_FTFont, text[i]);

			if (!g)
			{
				std::cout << "[Font] getOffsets() - Glyph \"g\" is null." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			float height = glyph->height / scale.y;
			float offset = glyph->offset_y / scale.y - height;

			if (offset < yo)
				yo = offset;
		}

		return math::Vector2f(glyph->offset_x / scale.x, yo);
	}

	float Font::getWidth(const String& text) const
	{
		using namespace ftgl;

		float width = 0.0f;
		const math::Vector2f& scale = m_Scale;

		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* g = texture_font_get_glyph(m_FTFont, text[i]);

			if (!g)
			{
				std::cout << "[Font] getOffsets() - Glyph \"g\" is null." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			if (i > 0)
			{
				float kerning = texture_glyph_get_kerning(g, text[i - 1]);
				width += kerning / scale.x;
			}

			width += g->advance_x / scale.x;
		}

		return width;
	}

	float Font::getHeight(const String& text) const
	{
		using namespace ftgl;

		float min = 0.0f;
		float max = 0.0f;
		const math::Vector2f& scale = m_Scale;

		for (int i = 0; i < text.size(); i++)
		{
			texture_glyph_t* g = texture_font_get_glyph(m_FTFont, text[i]);

			if (!g)
			{
				std::cout << "[Font] getOffsets() - Glyph \"g\" is null." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			float height = g->height / scale.y;
			float offset = g->offset_y / scale.y - height;
			
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}

		return abs(min) + abs(max);
	}

	math::Vector2f Font::getSize(const String& text) const
	{
		return math::Vector2f(getWidth(text), getHeight(text));
	}

	math::Rectangle Font::getBounds(const String& text) const
	{
		return math::Rectangle(0.0f, 0.0f, getWidth(text), getHeight(text));
	}

} }