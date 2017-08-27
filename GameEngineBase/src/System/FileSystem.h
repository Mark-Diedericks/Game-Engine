#pragma once

#include "Common.h"
#include "CustomString.h"

namespace gebase {
	class GE_API FileSystem {
	public:
		static bool FileExists(const String& path);
		static int64 GetFileSize(const String& path);

		static byte* ReadFileBytes(const String& path);
		static bool ReadFileBuffer(const String& path, void* buffer, int64 size = -1);
		static String ReadFileString(const String& path);

		static bool WriteFileBytes(const String& path, byte* buffer);
		static bool WriteFileString(const String& path, const String& text);

	};
}