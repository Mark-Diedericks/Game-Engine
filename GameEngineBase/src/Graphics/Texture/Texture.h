#pragma once

#include "ge.h"
#include "Common.h"
#include "CustomString.h"
#include "Backend/API/APITexture.h"

namespace gebase { namespace graphics {

	class GE_API Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(uint slot = 0) const = 0;
		virtual void Unbind(uint slot = 0) const = 0;

		virtual const String& getName() const = 0;
		virtual const String& getFilepath() const = 0;

		static void setWrap(API::TextureWrap wrap) { API::APITexture::setWrap(wrap); }
		static void setFilter(API::TextureFilter filter) { API::APITexture::setFilter(filter); }

		virtual API::APITexture* getInstance() = 0;
	};

} }