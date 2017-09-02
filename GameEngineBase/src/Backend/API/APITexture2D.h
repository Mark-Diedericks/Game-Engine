#pragma once

#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	class APITexture2D : public APITexture
	{
	public:
		virtual byte* getPixelData() = 0;

		virtual void setData(const void* pixels) = 0;
		virtual void setData(uint color) = 0;

		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;

		static APITexture2D* Create(uint width, uint height, TextureParameters parameters = TextureParameters());

		static APITexture2D* CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters = TextureParameters());
	};

} } }