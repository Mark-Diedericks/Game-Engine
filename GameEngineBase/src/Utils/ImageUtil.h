#pragma once

#include <string>
#include <xstring>
#include "Common.h"
#include "Types.h"
#include "CustomString.h"

#ifdef GELoadImage
#undef GELoadImage
#endif

namespace gebase {
	
	GE_API byte* GELoadImage(const char* filename, uint* width = nullptr, uint* height = nullptr, uint* bits = nullptr, bool flipY = false);
	GE_API byte* GELoadImage(const String& filename, uint* width = nullptr, uint* height = nullptr, uint* bits = nullptr, bool flipY = false);

}