#pragma once

#include "String.h"
#include "Common.h"

namespace utils {
	struct GE_API FileUtils {
	public:
		static inline String read_file(const char* filepath) {
			FILE* file = fopen(filepath, "rt");

			if (file == nullptr)
				std::cout << "Could not open file " << filepath << "." << std::endl;

			fseek(file, 0, SEEK_END);
			int32 length = ftell(file);

			String result(length, 0);

			fseek(file, 0, SEEK_SET);
			fread(&result[0], 1, length, file);
			fclose(file);

			result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());

			return result;
		}
	};
}
