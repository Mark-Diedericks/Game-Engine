#include "ge.h"
#include "LogStream.h"
#include "Timer.h"
#include "System\FileSystem.h"

namespace gebase { namespace utils {

#define GE_PRINT(x) std::cout << x;

	LogStream::LogStream(const String& fileLocation, const String& identifyingTag, const bool printToConsole, const bool logPerformance) : m_FileStream(fileLocation, std::ios_base::out), m_FileLocation(fileLocation), m_IdentifyingTag(identifyingTag), m_PrintConsole(printToConsole), m_LogPerformance(logPerformance)
	{
		if (m_FileStream.bad())
			GE_PRINT(("[ERROR] [LogStream] Write() - Cannot open file: " + m_FileLocation + '\n').c_str());
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
			m_FileStream << text;
			m_FileStream.flush();
		}
		else
		{
			GE_PRINT(("[ERROR] [LogStream] Write() - Cannot write to file: " + m_FileLocation + '\n').c_str());
		}

		if(m_PrintConsole)
			GE_PRINT(("[" + m_IdentifyingTag + "] " + text + '\n').c_str());
	}

	void LogStream::Write(const char* text)
	{
		if (m_LogPerformance)
		{
			gebase::Timer* timer = new gebase::Timer();
			timer->Reset();
			WritePriv(text);
			GE_PRINT(("[PERFORMANCE] (LogStream) WriteLine(): " + StringFormat::Float(timer->ElapsedMillis()) + "ms" + '\n').c_str());
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
			m_FileStream << text << std::endl;
			m_FileStream.flush();
		}
		else
		{
			GE_PRINT(("[ERROR] [LogStream] Write() - Cannot write to file: " + m_FileLocation + '\n').c_str());
		}

		if (m_PrintConsole)
			GE_PRINT(("[" + m_IdentifyingTag + "] " + text + '\n').c_str());
	}

	void LogStream::WriteLine(const char* text)
	{
		if (m_LogPerformance)
		{
			gebase::Timer* timer = new gebase::Timer();
			timer->Reset();
			WriteLinePriv(text); 
			GE_PRINT(("[PERFORMANCE] (LogStream) WriteLine(): " + StringFormat::Float(timer->ElapsedMillis()) + "ms" + '\n').c_str());
			delete timer;
		}
		else
		{
			WriteLinePriv(text);
		}
	}

} }