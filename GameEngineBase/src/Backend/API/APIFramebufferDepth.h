#pragma once

#include "APIFramebuffer.h"

namespace gebase { namespace graphics { namespace API {

	class APIFramebufferDepth : public APIFramebuffer
	{
	public:
		static APIFramebufferDepth* Create(uint width, uint height);

		virtual uint16* getPixelData() = 0;
		virtual void setData(const void* data) = 0;
	};

} } }