#include "Pch.h"
#include "VirtualFileSystem.h"

#include "Memory.h"
#include "FileSystem.h"

namespace gebase {
	VirtualFileSystem* VirtualFileSystem::s_Instance = nullptr;

	void VirtualFileSystem::Create() {
		s_Instance = genew VirtualFileSystem();
	}

	void VirtualFileSystem::Destroy() {
		gedel s_Instance;
	}

	void VirtualFileSystem::Mount(const String& virtualPath, const String& actualPath) {
		if (!s_Instance)
		{
			std::cout << "[VirtualFileSystem] Mount() - Instance is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		m_MountPoints[virtualPath].push_back(actualPath);
	}

	void VirtualFileSystem::Unmount(const String& path) {
		if (!s_Instance)
		{
			std::cout << "[VirtualFileSystem] Unount() - Instance is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
		
		m_MountPoints[path].clear();
	}

	bool VirtualFileSystem::ResolveActualPath(const String& path, String& outActualPath) {
		if (path[0] != '/')
		{
			outActualPath = path;
			return FileSystem::FileExists(path);
		}

		std::vector<String> dirs = SplitString(path, '/');
		const String& virtualDir = dirs.front();

		if (m_MountPoints.find(virtualDir) == m_MountPoints.end() || m_MountPoints[virtualDir].empty())
			return false;

		String rem = path.substr(virtualDir.size() + 1, path.size() - virtualDir.size());

		for (const String& pp : m_MountPoints[virtualDir])
		{
			String npath = pp + rem;
			if(FileSystem::FileExists(npath)) 
			{
				outActualPath = npath;
				return true;
			}
		}

		return false;
	}

	byte* VirtualFileSystem::ReadFileBytes(const String& path) {
		String ap;
		return ResolveActualPath(path, ap) ? FileSystem::ReadFileBytes(ap) : nullptr;
	}

	String VirtualFileSystem::ReadFileString(const String& path) {
		String ap;
		return ResolveActualPath(path, ap) ? FileSystem::ReadFileString(ap) : nullptr;
	}

	bool VirtualFileSystem::WriteFileBytes(const String& path, byte* buffer) {
		String ap;
		return ResolveActualPath(path, ap) ? FileSystem::WriteFileBytes(ap, buffer) : false;
	}

	bool VirtualFileSystem::WriteFileString(const String& path, const String& text) {
		String ap;
		return ResolveActualPath(path, ap) ? FileSystem::WriteFileString(ap, text) : false;
	}
}