#pragma once

#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	class APITextureDepth : public APITexture
	{
	public:
		static APITextureDepth* Create(uint width, uint height);
	};

} } }