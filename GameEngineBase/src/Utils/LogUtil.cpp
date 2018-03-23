#include "ge.h"
#include "LogUtil.h"

#include "System\FileSystem.h"

#include <iostream>
#include <ctime>

namespace gebase { namespace utils {

	std::vector<LogStream*> LogUtil::m_LogStreams;

	void LogUtil::Create(const String& directory)
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d_%m_%Y %I_%M_%S", timeinfo);
		std::string timestring(buffer);

		const String& dir = directory + "\\" + timestring +"\\";
		FileSystem::MakeDirectory(dir);

		Add("CRITICAL", dir + "critical.txt");
		Add("ERROR", dir + "error.txt");
		Add("WARNING", dir + "warning.txt");
		Add("INFO", dir + "info.txt");
		Add("PERFORMANCE", dir + "performance.txt");
		Add("DEBUG", dir + "debug.txt");
	}

	LogStream* LogUtil::Add(LogStream* stream)
	{
		m_LogStreams.push_back(stream);
		return stream;
	}

	LogStream* LogUtil::Get(const String& identifyingTag)
	{
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