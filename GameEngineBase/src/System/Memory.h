#pragma once
#pragma warning(disable:4595)

#include "Allocator.h"

#define genew new(__FILE__, __LINE__)
#define gedel delete


inline void* operator new(size_t size) { return gebase::Allocator::Allocate(size); }

inline void* operator new[](size_t size) { return gebase::Allocator::Allocate(size); }

inline void operator delete(void* block) { gebase::Allocator::Free(block); }

inline void operator delete[](void* block) { gebase::Allocator::Free(block); }


inline void* operator new(size_t size, const char* file, uint line) { return gebase::Allocator::Allocate(size); }

inline void* operator new[](size_t size, const char* file, uint line) { return gebase::Allocator::Allocate(size); }

inline void operator delete(void* block, const char* file, uint line) { gebase::Allocator::Free(block); }

inline void operator delete[](void* block, const char* file, uint line) { gebase::Allocator::Free(block); }

#pragma warning(default : 4595)