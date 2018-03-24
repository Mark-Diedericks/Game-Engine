#include "ge.h"
#include "LogStream.h"
#include "Timer.h"
#include "System\FileSystem.h"

namespace gebase { namespace utils {

	LogStream::LogStream(const String& fileLocation, const String& identifyingTag, const bool printToConsole, const bool logPerformance) : m_FileStream(fileLocation), m_FileLocation(fileLocation), m_IdentifyingTag(identifyingTag), m_PrintConsole(printToConsole), m_LogPerformance(logPerformance)
	{
		if(m_FileStream.bad())
			std::cout << "[ERROR] [LogStream] Write() - Cannot open file: " << m_FileLocation.c_str() << std::endl;
	}

	LogStream::~LogStream()
	{
		if(m_FileStream.is_open())
			m_FileStream.close();
	}

	void LogStream::WritePriv(const char* text)
	{
		if (m_FileStream.is_open())
		{
			m_FileStream << ("[" + m_IdentifyingTag + "] " + text);
			m_FileStream.flush();
		}
		else
		{
			std::cout << "[ERROR] [LogStream] Write() - Cannot write to file: " << m_FileLocation.c_str() << std::endl;
		}

		if(m_PrintConsole)
			std::cout << "[" + m_IdentifyingTag + "] " + text;
	}

	void LogStream::Write(const char* text)
	{
		if (m_LogPerformance)
		{
			gebase::Timer* timer = new gebase::Timer();
			timer->Reset();
			WritePriv(text);
			std::cout << ("[PERFORMANCE] (LogStream) WriteLine(): " + StringFormat::Float(timer->ElapsedMillis()) + "ms").c_str() << std::endl;
			delete timer;
		}
		else
		{
			WritePriv(text);
		}
	}

	void LogStream::WriteLinePriv(const char* text)
	{
		if (m_FileStream.is_open())
		{
			m_FileStream << ("[" + m_IdentifyingTag + "] " + text + '\n');
			m_FileStream.flush();
		}
		else
		{
			std::cout << "[ERROR] [LogStream] Write() - Cannot write to file: " << m_FileLocation.c_str() << std::endl;
		}

		if (m_PrintConsole)
			std::cout << "[" + m_IdentifyingTag + "] " + text << std::endl;
	}

	void LogStream::WriteLine(const char* text)
	{
		if (m_LogPerformance)
		{
			gebase::Timer* timer = new gebase::Timer();
			timer->Reset();
			WriteLinePriv(text); 
			std::cout << ("[PERFORMANCE] (LogStream) WriteLine(): " + StringFormat::Float(timer->ElapsedMillis()) + "ms").c_str() << std::endl;
			delete timer;
		}
		else
		{
			WriteLinePriv(text);
		}
	}

} }