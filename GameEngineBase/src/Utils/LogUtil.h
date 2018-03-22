#pragma once

#include "Common.h"
#include "LogStream.h"
#include <vector>

namespace gebase { namespace utils {

	class GE_API LogUtil
	{
	private:
		static std::vector<LogStream*> m_LogStreams;
		LogUtil() { }
	public:
		static void Create(const String& directory);

		static LogStream* Add(LogStream* stream);
		inline static LogStream* Add(const String& identifyingTag, const String& filepath) { return Add(new LogStream(filepath, identifyingTag)); }
		static LogStream* Get(const String& identifyingTag);

		static void Write(const String& identifyingTag, const String& text) { Write(identifyingTag, text.c_str()); }
		static void Write(const String& identifyingTag, const char* text);

		inline static void WriteLine(const String& identifyingTag, const String& text) { WriteLine(identifyingTag, text.c_str()); }
		static void WriteLine(const String& identifyingTag, const char* text);

		static void Clean();
	};

} }