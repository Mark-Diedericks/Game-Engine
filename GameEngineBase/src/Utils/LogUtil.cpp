#include "ge.h"
#include "LogUtil.h"

#include "System\FileSystem.h"

#include <iostream>
#include <ctime>

namespace gebase
{
	void PerfLogCall(const char* function, const char* file, int32 line, float time)
	{
		gebase::utils::LogUtil::WriteLine("PERFORMANCE", "(" + (String)file + " " + std::to_string(line) + ") " + (String)function + ": " + StringFormat::Float(time) + "ms");
	}
}

namespace gebase { namespace utils {

	std::vector<LogStream*> LogUtil::m_LogStreams;

	void LogUtil::Create(const String& directory)
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d_%m_%Y %H_%M_%S", timeinfo);
		std::string timestring(buffer);

		const String& dir = directory + "\\" + timestring +"\\";
		FileSystem::MakeDirectory(dir);

		Add("CRITICAL", dir + "critical.txt", true, true);
		Add("ERROR", dir + "error.txt", true, true);

#ifdef GE_DEBUG
		Add("WARNING", dir + "warning.txt", true, false);
		Add("INFO", dir + "info.txt", true, false);
		Add("PERFORMANCE", dir + "performance.txt", false, false);
		Add("MEMORY", dir + "memory.txt", false, false);
		Add("DEBUG", dir + "debug.txt", true, false);
#endif
	}

	LogStream* LogUtil::Add(LogStream* stream)
	{
		m_LogStreams.push_back(stream);
		return stream;
	}

	LogStream* LogUtil::Get(const String& identifyingTag)
	{
#ifndef GE_DEBUG
		if (identifyingTag == "WARNING" || identifyingTag == "INFO" || identifyingTag == "PERFORMANCE" || identifyingTag == "DEBUG")
			return nullptr;
#endif

		for (uint i = 0; i < m_LogStreams.size(); i++)
			if (m_LogStreams[i]->GetIdentifyingTag() == identifyingTag)
				return m_LogStreams[i];

		std::cout << "[ERROR] [LogUtil] Get() - Could not locate stream: " << identifyingTag.c_str() << std::endl;
		return nullptr;
	}

	void LogUtil::Clean()
	{
		for (uint i = 0; i < m_LogStreams.size(); i++)
			delete m_LogStreams[i];
	}

	void LogUtil::Write(const String& identifyingTag, const char* text)
	{
		LogStream* stream = Get(identifyingTag);

		if (stream != nullptr)
			stream->Write(text);
	}

	void LogUtil::WriteLine(const String& identifyingTag, const char* text)
	{
		LogStream* stream = Get(identifyingTag);

		if (stream != nullptr)
			stream->WriteLine(text);
	}

} }