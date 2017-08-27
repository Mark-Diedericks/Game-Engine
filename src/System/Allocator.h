#pragma once

#include "Common.h"

namespace gebase {
	class GE_API Allocator {
	public:
		static void* Allocate(size_t size);
		static void Free(void* block);
	};
}