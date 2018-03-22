#pragma once

#include "Common.h"
#include "CustomString.h"

#include <iostream>
#include <fstream>

namespace gebase { namespace utils {

	class GE_API LogStream
	{
	private:
		String m_FileLocation;
		String m_IdentifyingTag;
		std::ofstream m_FileStream;
	public:
		LogStream(const String& fileLocation, const String& identifyingTag);
		~LogStream();

		inline const String GetIdentifyingTag() { return m_IdentifyingTag; }

		inline void WriteLine(const String& text) { WriteLine(text.c_str()); }
		void WriteLine(const char* text);

		inline void Write(const String& text) { Write(text.c_str()); }
		void Write(const char* text);
	};

} }