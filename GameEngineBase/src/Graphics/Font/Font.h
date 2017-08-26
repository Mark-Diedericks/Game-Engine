#pragma once

#include "Common.h"
#include "Types.h"
#include "Math/Maths.h"
#include "Graphics/Texture/Texture2D.h"

namespace ftgl 
{
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace gebase { namespace graphics {

	class GE_API Font
	{
	private:
		ftgl::texture_atlas_t* m_FTAtlas;
		ftgl::texture_font_t* m_FTFont;

		float m_Size;
		math::Vector2f m_Scale;

		String m_Name;
		String m_Filepath;

		mutable Texture2D* m_Texture;

		void UpdateAtlas() const;
	public:
		Font(const String& name, const String& filepath, float size);
		Font(const String& name, const byte* data, uint datasize, float size);

		inline void setScale(float x, float y) { m_Scale = math::Vector2f(x, y); }
		inline void setScale(const math::Vector2f& scale) { m_Scale = scale; }
		inline const math::Vector2f& getScale() const { return m_Scale; }

		inline const String& getName() const { return m_Name; }
		inline const String& getFilepath() const { return m_Filepath; }

		inline ftgl::texture_font_t* getTFFont() const { return m_FTFont; }
		inline float getFontSize() const { return m_Size; }

		float getWidth(const String& text) const;
		float getHeight(const String& height) const;

		math::Vector2f getOffsets(const String& text) const;
		math::Vector2f getSize(const String& text) const;

		math::Rectangle getBounds(const String& text) const;
		Texture2D* getTexture() const;
	};

} }