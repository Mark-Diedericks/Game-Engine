#include "ge.h"
#include "CustomString.h"

namespace gebase {
	char* StringFormat::m_Buffer = new char[STRING_FORMAT_BUFFER_SIZE];

	std::vector<String> SplitString(const String& str, const String& delimeters) 
	{ 
		size_t start = 0;
		size_t end = str.find_first_of(delimeters);

		std::vector<String> result;

		while (end <= String::npos) {
			String token = str.substr(start, end - start);

			if (!token.empty())
				result.push_back(token);

			if (end == String::npos)
				break;

			start = end + 1;
			end = str.find_first_of(delimeters, start);
		}

		return result;
	}

	std::vector<String> SplitString(const String& str, const char delimeter) 
	{ 
		return SplitString(str, String(1, delimeter));
	}

	std::vector<String> Tokenize(const String& str) 
	{ 
		return SplitString(str, " \t\n");
	}

	std::vector<String> GetLines(const String& str) 
	{ 
		return SplitString(str, "\n");
	}

	const char* FindToken(const char* str, const String& token) 
	{ 
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]);
			bool right = !t[token.size()] || isspace(t[token.size()]);
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}

	const char* FindToken(const String& str, const String& token) 
	{ 
		return FindToken(str.c_str(), token);
	}

	int32 IndexOf(const String& str, const String& search, uint offset) 
	{ 
		const char* cstr = str.c_str() + offset;
		const char* found = strstr(cstr, search.c_str());

		if (found == nullptr)
			return -1;

		return (int32)(found - cstr) + offset;
	}

	int32 IndexOf(const String& str, const char* search, uint offset)
	{
		return IndexOf(str, String(search), offset);
	}

	String Substring(const String& str, uint start, uint length) 
	{ 
		return str.substr(start, length);
	}

	String Remove(const String& str, uint start, uint length) 
	{
		String result = str;
		return result.erase(start, length);
	}

	String GetBlock(const char* str, const char** outPosition) 
	{ 
		const char* end = strstr(str, "}");

		if (!end)
			return String(str);

		if (outPosition)
			*outPosition = end;

		uint length = end - str + 1;

		return String(str, length);
	}

	String GetBlock(const String& string, uint offset) 
	{ 
		const char* str = string.c_str() + offset;
		return GetBlock(str);
	}

	String GetStatement(const char* str, const char** outPosition) 
	{ 
		const char* end = strstr(str, ";");

		if (!end)
			return String(str);

		if (outPosition)
			*outPosition = end;

		uint length = end - str + 1;

		return String(str, length);
	}

	bool Contains(const String& str, const String& chars) 
	{ 
		return str.find(chars) != String::npos;
	}

	bool StartsWith(const String& str, const String& chars) 
	{ 
		return str.find(chars) == 0;
	}

	bool EndsWith(const String& str, const String& chars) 
	{ 
		String result = str;
		std::reverse(result.begin(), result.end());
		return result.find(chars) == 0;

	}

	int32 NextInt(const String& str)
	{
		const char* cstr = str.c_str();

		for (uint i = 0; i < str.size(); i++)
			if (isdigit(str[i]))
				return atoi(&str[i]);

		return -1;
	}
}