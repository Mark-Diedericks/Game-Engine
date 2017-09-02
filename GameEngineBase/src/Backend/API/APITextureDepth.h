#pragma once

#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	class APITextureDepth : public APITexture
	{
	public:
		static APITextureDepth* Create(uint width, uint height);

		virtual uint16* getPixelData() = 0;
		virtual void setData(const void* data) = 0;
	};

} } }