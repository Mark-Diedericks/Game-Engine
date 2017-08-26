#pragma once

#include "Pch.h"
#include "String.h"

namespace gebase {
	class GE_API VirtualFileSystem {
	private:
		static VirtualFileSystem* m_Instance;
		std::unordered_map<String, std::vector<String>> m_MountPoints;
	public:
		void Mount(const String& virtualPath, const String& actualPath);
		void Unmount(const String& path);
		
		bool ResolveActualPath(const String& path, String& outActualPath);

		byte* ReadFileBytes(const String& path);
		String ReadFileString(const String& path);

		bool WriteFileBytes(const String& path, byte* buffer);
		bool WriteFileString(const String& path, const String& text);

		static void Create();
		static void Destroy();

		inline static VirtualFileSystem* Get() { return m_Instance; }
	};
}