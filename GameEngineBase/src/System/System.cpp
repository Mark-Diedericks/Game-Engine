#include "ge.h"
#include "System.h"
#include "VirtualFileSystem.h"
#include "Utils\LogUtil.h"

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
		utils::LogUtil::WriteLine("INFO", "--------------------------");
		utils::LogUtil::WriteLine("INFO", "    SYSTEM INFORMATION    ");
		utils::LogUtil::WriteLine("INFO", "--------------------------");

		//s_SystemInfo.processorInfo.Log();
		s_SystemInfo.memoryInfo.Log();
		//s_SystemInfo.graphicsInfo.Log();
	}
}