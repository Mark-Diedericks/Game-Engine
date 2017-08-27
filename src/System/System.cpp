#include "ge.h"
#include "System.h"
#include "VirtualFileSystem.h"

namespace gebase {

	SystemInfo System::s_SystemInfo;

	void System::Create()
	{
		MemoryManager::Create();
		VirtualFileSystem::Create();

		s_SystemInfo.memoryInfo = MemoryManager::Get()->GetSystemInfo();
		LogSystemInfo();
	}

	void System::Destroy()
	{
		VirtualFileSystem::Destroy();
		MemoryManager::Destroy();
	}

	void System::LogSystemInfo()
	{
		std::cout << "--------------------------" << std::endl;
		std::cout << "    SYSTEM INFORMATION    " << std::endl;
		std::cout << "--------------------------" << std::endl;

		//s_SystemInfo.processorInfo.Log();
		s_SystemInfo.memoryInfo.Log();
		//s_SystemInfo.graphicsInfo.Log();
	}
}