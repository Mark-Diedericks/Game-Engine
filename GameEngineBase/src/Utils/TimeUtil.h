#pragma once

#include <chrono>
#include "Common.h"

namespace utils {
	class GE_API TimeUtil {
	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> m_timestamp;
	public:
		inline static void set() { m_timestamp = std::chrono::high_resolution_clock::now(); }
		inline static double get() { return ((std::chrono::high_resolution_clock::now() - m_timestamp).count()) / 1000000.0; }
	};
}