#pragma once

#include "Common.h"

namespace gebase {
	class GE_API Allocator {
	public:
		static void* AllocateDebug(size_t size, const char* file, uint line);
		static void* Allocate(size_t size);

		static void FreeDebug(void* block, const char* file, uint line);
		static void Free(void* block);
	};
}