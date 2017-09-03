#pragma once

#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	class APITextureDepth : public APITexture
	{
	public:
		static APITextureDepth* Create(uint width, uint height);

		virtual void getPixelData(uint16* pixels) = 0;
		virtual uint getSize() const = 0;

		virtual void setData(const uint16* data) = 0;
	};

} } }