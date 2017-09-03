#pragma once

#include "APIFramebuffer.h"

namespace gebase { namespace graphics { namespace API {

	class APIFramebufferDepth : public APIFramebuffer
	{
	public:
		static APIFramebufferDepth* Create(uint width, uint height);

		virtual void getPixelData(uint16* data) = 0;
		virtual void setData(const uint16* data) = 0;
	};

} } }