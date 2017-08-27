#pragma once

#include <string>
#include <xstring>
#include "Common.h"
#include "Types.h"
#include "CustomString.h"

#ifdef LoadImage
#undef LoadImage
#endif

namespace gebase {
	
	GE_API byte* LoadImage(const char* filename, uint* width = nullptr, uint* height = nullptr, uint* bits = nullptr, bool flipY = false);
	GE_API byte* LoadImage(const String& filename, uint* width = nullptr, uint* height = nullptr, uint* bits = nullptr, bool flipY = false);

}