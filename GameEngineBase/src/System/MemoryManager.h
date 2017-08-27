#pragma once

#include "CustomString.h"

namespace gebase {
	struct GE_API SystemMemoryInfo {
		int64 apm;
		int64 tpm;

		int64 avm;
		int64 tvm;

		void Log();
	};

	struct GE_API MemoryInfo {
		int64 tAllocated;
		int64 tFreed;

		int64 cUsed;
		int64 tAllocations;

		MemoryInfo() : tAllocated(0), tFreed(0), cUsed(0), tAllocations(0) {}
	};

	class GE_API MemoryManager {
	public:
		static MemoryManager* m_Instance;
		MemoryInfo m_MemoryInfo;
	public:
		MemoryManager();

		static void Create();
		static void Destroy();

		static MemoryManager* Get();
		inline MemoryInfo GetMemoryInfo() const { return m_MemoryInfo; }
		SystemMemoryInfo GetSystemInfo();
		static String BytesToString(int64 bytes);
	};
}
