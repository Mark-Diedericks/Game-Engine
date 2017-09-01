#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "Common.h"

namespace gebase { namespace math {

	GE_API inline float toRadians(float degrees)
	{
		return (float)(degrees * (GE_PI / 180.0f));
	}

	GE_API inline float toDegrees(float radians)
	{
		return (float)(radians * (180.0f / GE_PI));
	}

	GE_API inline int32 sign(float value)
	{
		return (value > 0) - (value < 0);
	}

	GE_API inline float sin(float angle)
	{
		return ::sin(angle);
	}

	GE_API inline float cos(float angle)
	{
		return ::cos(angle);
	}

	GE_API inline float tan(float angle)
	{
		return ::tan(angle);
	}

	GE_API inline float sqrt(float value)
	{
		return ::sqrt(value);
	}

	GE_API inline float rsqrt(float value)
	{
		return 1.0f / ::sqrt(value);
	}

	GE_API inline float asin(float value)
	{
		return ::asin(value);
	}

	GE_API inline float acos(float value)
	{
		return ::acos(value);
	}

	GE_API inline float atan(float value)
	{
		return ::atan(value);
	}

	GE_API inline float atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

	GE_API inline float _min(float value, float minimum)
	{
		return (value < minimum) ? minimum : value;
	}

	GE_API inline float _max(float value, float maximum)
	{
		return (value > maximum) ? maximum : value;
	}

	GE_API inline float clamp(float value, float minimum, float maximum)
	{
		return (value > minimum) ? (value < maximum) ? value : maximum : minimum;
	}

} }