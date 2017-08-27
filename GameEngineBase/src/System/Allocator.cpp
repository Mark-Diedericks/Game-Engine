#include "ge.h"
#include "Common.h"

#include "Allocator.h"
#include "MemoryManager.h"
#include <malloc.h>

#define GE_MEMORY_ALIGNMENT 16
#define GE_ALLOC(size) _aligned_malloc(size, GE_MEMORY_ALIGNMENT)
#define GE_FREE(block) _aligned_free(block);

namespace gebase {

	void* Allocator::Allocate(size_t size) {
		gebase::MemoryManager::Get()->m_MemoryInfo.tAllocated += size;
		gebase::MemoryManager::Get()->m_MemoryInfo.cUsed += size;
		gebase::MemoryManager::Get()->m_MemoryInfo.tAllocations++;

		size_t asize = size + sizeof(size_t);
		byte* result = (byte*)GE_ALLOC(asize);

		std::memset(result, 0, asize);
		std::memcpy(result, &size, sizeof(size_t));

		result += sizeof(size_t);
		return result;
	}

	void Allocator::Free(void* block) {
		byte* mem = ((byte*)block) - sizeof(size_t);
		size_t size = *(size_t*)mem;


		gebase::MemoryManager::Get()->m_MemoryInfo.tFreed += size;
		gebase::MemoryManager::Get()->m_MemoryInfo.cUsed -= size;

		GE_FREE(mem);
	}

}