#pragma once

#include "Texture.h"
#include "Backend/API/APITextureDepth.h"

namespace gebase { namespace graphics {

	class TextureDepth : public Texture
	{
	private:
		API::APITextureDepth* m_Instance;
	public:
		static TextureDepth* Create(uint width, uint height);

		inline void Bind(uint slot = 0) const override { m_Instance->Bind(slot); }
		inline void Unbind(uint slot = 0) const override { m_Instance->Unbind(slot); }

		inline const String& getName() const override { return m_Instance->getName(); }
		inline const String& getFilepath() const override { return m_Instance->getFilepath(); }

		inline API::APITexture* getInstance() override { return m_Instance; }
	};

} }