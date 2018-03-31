#include "ge.h"
#include "Common.h"
#include "Glyph.h"

#include "Utils/LogUtil.h"

#include <freetype-gl/freetype-gl.h>

namespace gebase { namespace graphics {

	Glyph::Glyph(const char c, const Font& font, float* xoffset, const char previous)
	{
		using namespace ftgl;

		const math::Vector2f& scale = font.getScale();
		float x = *xoffset;

		texture_font_t* ftFont = font.getFTFont();
		texture_glyph_t* glyph = texture_font_get_glyph(ftFont, c);

		if (glyph)
		{
			if (previous != NULL)
			{
				float kerning = texture_glyph_get_kerning(glyph, previous);
				x += kerning / scale.x;
			}

			m_X0 = x + glyph->offset_x / scale.x;
			m_Y0 = glyph->offset_y / scale.y;
			m_X1 = m_X0 + glyph->width / scale.x;
			m_Y1 = m_Y0 - glyph->height / scale.y;

			m_U0 = glyph->s0;
			m_V0 = glyph->t0;
			m_U1 = glyph->s1;
			m_V1 = glyph->t1;

			x += glyph->advance_x / scale.x;
		}

		*xoffset = x;
	}

} }