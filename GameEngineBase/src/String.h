#pragma once

#include <string>
#include <vector>
#include "Common.h"
#include "Types.h"

typedef std::string String;

namespace gebase {

#define STRING_FORMAT_BUFFER_SIZE 10240

	class GE_API StringFormat 
	{
	private:
		static char* m_Buffer;
	public:
		template<typename T>
		static String Hex(const T& input) {
			memset(m_Buffer, 0, STRING_FORMAT_BUFFER_SIZE);
			sprintf(m_Buffer, "%02x", input);
			return String(m_Buffer);
		}

		template<typename T>
		static String Hex(const T* input, uint size) {
			memset(m_Buffer, 0, STRING_FORMAT_BUFFER_SIZE);
			for (uint i = 0; i < size; i++)
				sprintf(m_Buffer + i * 3, "%02x", input[i]);
			return String(m_Buffer);
		}

		static String Float(const float input, uint place = 2) {
			memset(m_Buffer, 0, STRING_FORMAT_BUFFER_SIZE);
			sprintf(m_Buffer, "%.2f", input);
			return String(m_Buffer);
		}

		template<typename T>
		static String ToString(const T& input) {
			return std::to_string(input);
		}
	};

	std::vector<String> SplitString(const String& str, const String& delimeters);
	std::vector<String> SplitString(const String& str, const char delimeter);

	std::vector<String> Tokenize(const String& str);
	std::vector<String> GetLines(const String& str);
	
	const char* FindToken(const String& str, const String& token);
	const char* FindToken(const char* str, const String& token);

	int32 IndexOf(const String& str, const String& search, uint offset = 0);
	int32 IndexOf(const String& str, const char* search, uint offset = 0);

	String Substring(const String& str, uint start, uint length);

	String Remove(const String& str, uint start, uint length);

	String GetBlock(const char* str, const char** outPosition = nullptr);
	String GetBlock(const String& str, uint offset = 0);

	String GetStatement(const char* str, const char** outPosition = nullptr);

	bool Contains(const String& str, const String& chars);
	bool StartsWith(const String& str, const String& chars);
	bool EndsWith(const String& str, const String& chars);

	int32 NextInt(const String& str);
}