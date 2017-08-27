#include "ge.h"
#include "Vector4f.h"
#include "Matrix4f.h"

namespace gebase { namespace math {
	Vector4f::Vector4f(const float& ix, const float& iy, const float& iz, const float& iw) {
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	float Vector4f::getLength() const {
		return (float)::sqrt(x * x + y * y + z * z + w * w);
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

	Vector4f Vector4f::operator *(const Matrix4f& mat4f) const {
		return Vector4f(mat4f.rows[0].x * x + mat4f.rows[0].y * y + mat4f.rows[0].z * z + mat4f.rows[0].w * w, mat4f.rows[1].x * x + mat4f.rows[1].y * y + mat4f.rows[1].z * z + mat4f.rows[1].w * w, mat4f.rows[2].x * x + mat4f.rows[2].y * y + mat4f.rows[2].z * z + mat4f.rows[2].w * w, mat4f.rows[3].x * x + mat4f.rows[3].y * y + mat4f.rows[3].z * z + mat4f.rows[3].w * w);
	}

	Vector4f Vector4f::Mul(const Matrix4f& mat4f) const {
		return Vector4f(mat4f.rows[0].x * x + mat4f.rows[0].y * y + mat4f.rows[0].z * z + mat4f.rows[0].w * w, mat4f.rows[1].x * x + mat4f.rows[1].y * y + mat4f.rows[1].z * z + mat4f.rows[1].w * w, mat4f.rows[2].x * x + mat4f.rows[2].y * y + mat4f.rows[2].z * z + mat4f.rows[2].w * w, mat4f.rows[3].x * x + mat4f.rows[3].y * y + mat4f.rows[3].z * z + mat4f.rows[3].w * w);
	}
} }