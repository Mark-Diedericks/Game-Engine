#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "Common.h"

namespace gebase { namespace math {
	class GE_API Utils {
	public:
		static inline float toRadians(const float& degrees) {
			return (degrees * (float)M_PI) / 180.0f;
		}

		static inline float toDegrees(const float& radians) {
			return (radians * 180.0f) / (float)M_PI;
		}

		static inline float clamp(float in, float min, float max)
		{
			float out = in;

			if (out < min)
				out = min;

			if (out > max)
				out = max;

			return out;
		}
	};
} }