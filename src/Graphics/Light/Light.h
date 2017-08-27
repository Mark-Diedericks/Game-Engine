#pragma once

#include "Types.h"
#include "Math/Maths.h"

namespace gebase { namespace graphics {

	_declspec(align(16)) struct GE_API Light
	{
		math::Vector4f color;
		math::Vector3f position;
		float p0;
		math::Vector3f direction;
		float p1;
		math::Vector3f lightVector;
		float intensity;

		Light(const math::Vector3f& direction, float intensity = 1.0f, const math::Vector4f& color = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	};

} }