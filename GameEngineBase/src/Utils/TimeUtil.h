#pragma once

#include <chrono>
#include "Common.h"

namespace gebase { namespace utils {
	class GE_API TimeUtil {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Timestamp;
	public:
		inline void set() { m_Timestamp = std::chrono::high_resolution_clock::now(); }
		inline double get() { return ((std::chrono::high_resolution_clock::now() - m_Timestamp).count()) / 1000000.0; }
	};
} }