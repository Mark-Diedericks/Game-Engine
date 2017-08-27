#include "ge.h"
#include "Vector2f.h"
#include "Vector3f.h"

namespace gebase { namespace math {

	Vector2f::Vector2f(const float& ix, const float& iy) 
	{
		x = ix;
		y = iy;
	}

	Vector2f::Vector2f(const Vector3f& vector) : x(vector.x), y(vector.y)
	{
	}

	float Vector2f::getLength() const {
		return (float)::sqrt(x * x + y * y);
	}

	float Vector2f::dot(const Vector2f& vec2f) const {
		return x * vec2f.x + y * vec2f.y;
	}

	float Vector2f::cross(const Vector2f& vec2f) const {
		return x * vec2f.y - y * vec2f.x;
	}

	Vector2f Vector2f::refelect(const Vector2f& normal) const {
		return *this - (normal * (this->dot(normal) * 2));
	}

	Vector2f Vector2f::lerp(const Vector2f& vec2f, const float& factor) const {
		return ((vec2f - *this) * factor) - *this;
	}

	Vector2f Vector2f::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		return (Vector2f)*this;
	}
} }