#include "ge.h"
#include "LogStream.h"
#include "System\FileSystem.h"

namespace gebase { namespace utils {

	LogStream::LogStream(const String& fileLocation, const String& identifyingTag) : m_FileStream(fileLocation), m_FileLocation(fileLocation), m_IdentifyingTag(identifyingTag)
	{
		if(m_FileStream.bad())
			std::cout << "[ERROR] [LogStream] Write() - Cannot open file: " << m_FileLocation.c_str() << std::endl;
	}

	LogStream::~LogStream()
	{
		if(m_FileStream.is_open())
			m_FileStream.close();
	}

	void LogStream::Write(const char* text)
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

		std::cout << "[" + m_IdentifyingTag + "] " + text;
	}

	void LogStream::WriteLine(const char* text)
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

		std::cout << "[" + m_IdentifyingTag + "] " + text << std::endl;
	}

} }