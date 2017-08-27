#include "ge.h"
#include "MemoryManager.h"
#include "Memory.h"
#include "CustomString.h"

namespace gebase {
	MemoryManager* MemoryManager::m_Instance = nullptr;

	MemoryManager::MemoryManager() {
		//Redundant
	}

	void MemoryManager::Create() {
		//Redundant
	}

	void MemoryManager::Destroy() {
		gedel m_Instance;
	}

	MemoryManager* MemoryManager::Get() {

		if (m_Instance == nullptr)
		{
			m_Instance = (MemoryManager*)malloc(sizeof(MemoryManager));
			m_Instance = new(m_Instance) MemoryManager();
		}

		return m_Instance;
	}

	String MemoryManager::BytesToString(int64 bytes) {
		static const float kb = 1024;
		static const float mb = 1048576;
		static const float gb = 1073741824;

		if (bytes > gb)
			return StringFormat::Float(bytes / gb) + "GB";

		if (bytes > mb)
			return StringFormat::Float(bytes / mb) + "MB";

		if (bytes > kb)
			return StringFormat::Float(bytes / kb) + "KB";

		return StringFormat::Float((float)bytes) + "B";
	}

	void SystemMemoryInfo::Log() {
		std::cout << "Avaliable Physical Memory: " << MemoryManager::BytesToString(apm).c_str() << std::endl;
		std::cout << "Total Physical Memory: " << MemoryManager::BytesToString(tpm).c_str() << std::endl;

		std::cout << "Avaliable Virtual Memory: " << MemoryManager::BytesToString(avm).c_str() << std::endl;
		std::cout << "Total Virtual Memory: " << MemoryManager::BytesToString(tvm).c_str() << std::endl;
	}
}