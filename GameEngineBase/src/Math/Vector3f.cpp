#include "ge.h"
#include "Vector3f.h"
#include "Vector2f.h"

namespace gebase { namespace math {
	
	Vector3f::Vector3f(const float& ix, const float& iy, const float& iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	Vector3f::Vector3f(const Vector2f& vector) : x(vector.x), y(vector.y), z(0)
	{

	}

	float Vector3f::getLength() const {
		return (float)std::sqrt(x * x + y * y + z * z);
	}

	float Vector3f::dot(const Vector3f& vec3f) const {
		return x * vec3f.x + y * vec3f.y + z * vec3f.z;
	}

	Vector3f Vector3f::cross(const Vector3f& vec3f) const {
		float m_x = y * vec3f.z - z * vec3f.y;
		float y = z * vec3f.x - m_x * vec3f.z;
		float z = m_x * vec3f.y - y * vec3f.x;
		return Vector3f(m_x, y, z);
	}

	Vector3f Vector3f::refelect(const Vector3f& normal) const {
		return *this - (normal * (this->dot(normal) * 2));
	}

	Vector3f Vector3f::rotate(const Vector3f& axis, const float& angle) {
		const float sinAngle = (float)std::sin(-angle);
		const float cosAngle = (float)std::cos(-angle);
		return this->cross(axis * sinAngle) + (*this * cosAngle) + axis * this->dot(axis * (1 - cosAngle));
	}

	Vector3f Vector3f::lerp(const Vector3f& vec3f, const float& factor) const {
		return ((vec3f - *this) * factor) - *this;
	}

	Vector3f Vector3f::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		z /= l;
		return (Vector3f)*this;
	}
} }