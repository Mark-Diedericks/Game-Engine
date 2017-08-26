#pragma once

#include "Pch.h"
#include "Common.h"
#include "Texture.h"

namespace gebase { namespace graphics {

	class GE_API TextureManager
	{
	private:
		static std::vector<Texture*> m_Textures;
		TextureManager() { }
	public:
		static Texture* Add(Texture* texture);
		static Texture* Get(const String& name);

		static void Clean();
	};

} }