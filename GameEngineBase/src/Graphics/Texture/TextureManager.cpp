#include "Pch.h"
#include "TextureManager.h"

namespace gebase { namespace graphics {

	std::vector<Texture*> TextureManager::m_Textures;

	Texture* TextureManager::Add(Texture* texture)
	{
		m_Textures.push_back(texture);
		return texture;
	}

	Texture* TextureManager::Get(const String& name)
	{
		for (Texture* texture : m_Textures)
			if (texture->getName() == name)
				return texture;

		return nullptr;
	}

	void TextureManager::Clean()
	{
		for (uint i = 0; i < m_Textures.size(); i++)
			delete m_Textures[i];
	}

} }