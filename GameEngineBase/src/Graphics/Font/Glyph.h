#pragma once

#include "Common.h"
#include "Font.h"

namespace gebase { namespace graphics {

	class GE_API Glyph
	{
	private:
		float m_X0;
		float m_X1;

		float m_Y0;
		float m_Y1;

		float m_U0;
		float m_U1;

		float m_V0;
		float m_V1;
	public:
		Glyph(const char c, const Font& font, float* xoffset, const char previous);

		inline const float getX0() const { return m_X0; }
		inline const float getX1() const { return m_X1; }

		inline const float getY0() const { return m_Y0; }
		inline const float getY1() const { return m_Y1; }

		inline const float getU0() const { return m_U0; }
		inline const float getU1() const { return m_U1; }

		inline const float getV0() const { return m_V0; }
		inline const float getV1() const { return m_V1; }
	};

} }