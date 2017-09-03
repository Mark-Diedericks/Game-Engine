#pragma once

#include "Allocator.h"

#define genew new(__FILE__, __LINE__)
#define gedel delete

#pragma warning(disable : 4595)

inline void* operator new(size_t size) { return gebase::Allocator::Allocate(size); }

inline void* operator new[](size_t size) { return gebase::Allocator::Allocate(size); }

inline void operator delete(void* block) { gebase::Allocator::Free(block); }

inline void operator delete[](void* block) { gebase::Allocator::Free(block); }


inline void* operator new(size_t size, const char* file, uint line) { return gebase::Allocator::AllocateDebug(size, file, line); }

inline void* operator new[](size_t size, const char* file, uint line) { return gebase::Allocator::AllocateDebug(size, file, line); }

inline void operator delete(void* block, const char* file, uint line) { gebase::Allocator::FreeDebug(block, file, line); }

inline void operator delete[](void* block, const char* file, uint line) { gebase::Allocator::FreeDebug(block, file, line); }

#pragma warning(default : 4595)