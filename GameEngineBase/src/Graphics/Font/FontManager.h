#pragma once

#include "Common.h"
#include "Types.h"
#include "Font.h"

namespace gebase { namespace graphics {

	class GE_API FontManager
	{
	private:
		static std::vector<Font*> s_Fonts;
		static math::Vector2f s_Scale;

		FontManager() { }
	public:
		static void setScale(const math::Vector2f& scale);
		static inline const math::Vector2f& getScale() { return m_Scale; }

		static void Clean();
		static void Add(Font* font);

		static Font* Get();
		static Font* Get(const String& name);
		static Font* Get(uint size);
		static Font* Get(const String& name, uint size);
	};

} }