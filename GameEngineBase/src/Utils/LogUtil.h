#pragma once

#include "Common.h"
#include "LogStream.h"
#include "Timer.h"
#include <vector>

namespace gebase
{
	extern void PerfLogCall(const char* function, const char* fle, int32 line, float time);
}

#ifdef GE_DEBUG
#define GE_PERF(x)  {\
							gebase::Timer* timer = new gebase::Timer();\
							timer->Reset();\
							x;\
							gebase::PerfLogCall(#x, __FILE__, __LINE__, timer->ElapsedMillis());\
							delete timer;\
						}
#else
	#define GE_PERF(x) x;
#endif

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
		inline static LogStream* Add(const String& identifyingTag, const String& filepath, const bool printToConsole, const bool logPerformance) { return Add(new LogStream(filepath, identifyingTag, printToConsole, logPerformance)); }
		static LogStream* Get(const String& identifyingTag);

		static void Write(const String& identifyingTag, const String& text) { Write(identifyingTag, text.c_str()); }
		static void Write(const String& identifyingTag, const char* text);

		inline static void WriteLine(const String& identifyingTag, const String& text) { WriteLine(identifyingTag, text.c_str()); }
		static void WriteLine(const String& identifyingTag, const char* text);

		static void Clean();
	};

} }