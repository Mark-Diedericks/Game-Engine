#pragma once

#include <chrono>
#include "Common.h"

namespace gebase { namespace utils {
	class GE_API TimeUtil {
	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> s_Timestamp;
	public:
		inline static void set() { s_Timestamp = std::chrono::high_resolution_clock::now(); }
		inline static double get() { return ((std::chrono::high_resolution_clock::now() - s_Timestamp).count()) / 1000000.0; }
	};
} }