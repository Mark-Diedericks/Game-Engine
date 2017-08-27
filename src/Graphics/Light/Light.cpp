#include "ge.h"
#include "Light.h"

namespace gebase { namespace graphics {

	Light::Light(const math::Vector3f& direction, float intensity, const math::Vector4f& color) : direction(direction), intensity(intensity), color(color), p0(0.0f), p1(0.0f), position(math::Vector3f(0.0f, 0.0f, 0.0f)), lightVector(math::Vector3f(0.0f, 0.0f, 0.0f))
	{

	}

} }