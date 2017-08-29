#include "ge.h"
#include "Common.h"
#include "System/FileSystem.h"
#include "System/Memory.h"

#include <Windows.h>

namespace gebase {

	void CALLBACK FileIOCompletionRoutine(DWORD dwErrCode, DWORD dwNumBytesTransfered, LPOVERLAPPED lpOverlapped) { }

	static HANDLE OpenFileForReading(const String& path) {
		return CreateFile((LPCWSTR)path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	}

	static int64 GetFileSizeInternal(HANDLE file) {
		LARGE_INTEGER size;
		GetFileSizeEx(file, &size);
		return size.QuadPart;
	}

	static bool ReadFileInternal(HANDLE file, void* buffer, int64 size) {
		OVERLAPPED overlapped = { 0 };
		return ReadFileEx(file, buffer, (DWORD)size, &overlapped, FileIOCompletionRoutine);
	}

	bool FileSystem::FileExists(const String& path) {
		DWORD res = GetFileAttributes((LPCWSTR)path.c_str());

		if (res == INVALID_FILE_ATTRIBUTES)
			std::cout << "[Win32FileSystem] FileExists() - File has invalid attributes; " << path.c_str() << std::endl;

		DWORD lerror = GetLastError();
		
		if (lerror == ERROR_FILE_NOT_FOUND)
			std::cout << "[Win32FileSystem] FileExists() - File does not exist; " << path.c_str() << std::endl;

		return !(res == INVALID_FILE_ATTRIBUTES && lerror == ERROR_FILE_NOT_FOUND);
	}

	int64 FileSystem::GetFileSize(const String& path) {
		HANDLE file = OpenFileForReading(path);

		if (file == INVALID_HANDLE_VALUE)
			return -1;

		int64 result = GetFileSizeInternal(file);
		CloseHandle(file);

		return result;
	}

	byte* FileSystem::ReadFileBytes(const String& path) {
		HANDLE file = OpenFileForReading(path);

		if (file == INVALID_HANDLE_VALUE)
			return nullptr;

		int64 size = GetFileSizeInternal(file);
		byte* buffer = genew byte[(uint)size];

		bool result = ReadFileInternal(file, buffer, size);
		CloseHandle(file);

		if (!result)
			gedel buffer;

		return result ? buffer : nullptr;
	}

	bool FileSystem::ReadFileBuffer(const String& path, void* buffer, int64 size) {
		HANDLE file = OpenFileForReading(path);

		if (file == INVALID_HANDLE_VALUE)
			return false;

		if (size < 0)
			size = GetFileSizeInternal(file);

		bool result = ReadFileInternal(file, buffer, size);
		CloseHandle(file);
		return result;

	}

	String FileSystem::ReadFileString(const String& path) {
		HANDLE file = OpenFileForReading(path);

		if (file == INVALID_HANDLE_VALUE)
			return String();

		int64 size = GetFileSizeInternal(file);
		String fin((uint)size, 0);

		bool result = ReadFileInternal(file, &fin[0], size);
		CloseHandle(file);

		if (result)
			fin.erase(std::remove(fin.begin(), fin.end(), '\r'), fin.end());

		return result ? fin : String();
	}

	bool FileSystem::WriteFileBytes(const String& path, byte* buffer) {
		HANDLE file = CreateFile((LPCWSTR)path.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_NEW | OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
			return false;

		int64 size = GetFileSizeInternal(file);
		DWORD written;

		bool result = ::WriteFile(file, buffer, (DWORD)size, &written, NULL);
		CloseHandle(file);

		return result;
	}

	bool FileSystem::WriteFileString(const String& path, const String& text) {
		return WriteFileBytes(path, (byte*)&text[0]);
	}

}