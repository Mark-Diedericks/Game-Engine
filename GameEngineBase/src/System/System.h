#pragma once

#include "MemoryManager.h"

namespace gebase {

	struct SystemInfo
	{
		SystemMemoryInfo memoryInfo;
		//GraphicsInfo graphicsInfo;
		//ProcessorInfo processorInfo;
	};

	class System
	{
	private:
		static SystemInfo s_SystemInfo;
		static void LogSystemInfo();
	public:
		static void Create();
		static void Destroy();
		inline static SystemInfo getSystemInfo() { return s_SystemInfo; }
	};

}