#include "Pch.h"
#include "Vector4f.h"

namespace gebase { namespace math {
	Vector4f::Vector4f(const float& ix, const float& iy, const float& iz, const float& iw) {
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	float Vector4f::getLength() const {
		return (float)std::sqrt(x * x + y * y + z * z + w * w);
	}

	float Vector4f::dot(const Vector4f& vec4f) const {
		return x * vec4f.x + y * vec4f.y + z * vec4f.z + w * vec4f.w;
	}

	Vector4f Vector4f::refelect(const Vector4f& normal) const {
		return *this - (normal * (this->dot(normal) * 2));
	}


	Vector4f Vector4f::lerp(const Vector4f& vec4f, const float& factor) const {
		return ((vec4f - *this) * factor) - *this;
	}

	Vector4f Vector4f::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		z /= l;
		w /= l;
		return (Vector4f)*this;
	}
} }