#pragma once

#include "ge.h"
#include "Common.h"
#include "CustomString.h"
#include "Backend/API/APITexture.h"

namespace gebase { namespace graphics {

	class GE_API Texture
	{
	protected:
		static API::TextureWrap s_WrapMode;
		static API::TextureFilter s_FilterMode;
	public:
		virtual ~Texture() {}

		virtual void Bind(uint slot = 0) const = 0;
		virtual void Unbind(uint slot = 0) const = 0;

		virtual const String& getName() const = 0;
		virtual const String& getFilepath() const = 0;

		static void setWrap(API::TextureWrap wrap) { s_WrapMode = wrap; API::APITexture::setWrap(wrap); }
		static void setFilter(API::TextureFilter filter) { s_FilterMode = filter; API::APITexture::setFilter(filter); }

		virtual API::APITexture* getInstance() = 0;
	};

} }