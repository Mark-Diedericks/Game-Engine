#pragma once

#include <math.h>

#include "Common.h"
#include "Vector3f.h"

namespace gebase { namespace math {

	class Matrix4f;

	class GE_API Vector4f {
	public:
		float x, y, z, w;

		inline Vector4f() : x(0), y(0), z(0), w(0) {}
		Vector4f(const float& ix, const float& iy, const float& iz, const float& iw);
		Vector4f(const Vector3f& vector, const float& iw);
		float getLength() const;
		float dot(const Vector4f& vec4f) const;
		Vector4f normalize();
		Vector4f lerp(const Vector4f& vec4f, const float& factor) const;
		Vector4f refelect(const Vector4f& normal) const;

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
			this->z = f;
			this->w = f;
		}

		inline void operator =(const Vector4f& vec4f) {
			this->x = vec4f.x;
			this->y = vec4f.y;
			this->z = vec4f.z;
			this->w = vec4f.w;
		}

		inline bool operator ==(const Vector4f& vec4f) const {
			return this->x == vec4f.x && this->y == vec4f.y && this->z == vec4f.z && this->w == vec4f.w;
		}

		inline Vector4f& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			this->z += f;
			this->w += f;
			return *this;
		}

		inline Vector4f& operator +=(const Vector4f& vec4f) {
			this->x += vec4f.x;
			this->y += vec4f.y;
			this->z += vec4f.z;
			this->w += vec4f.w;
			return *this;
		}

		inline Vector4f& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			this->z -= f;
			this->w -= f;
			return *this;
		}

		inline Vector4f& operator -=(const Vector4f& vec4f) {
			this->x -= vec4f.x;
			this->y -= vec4f.y;
			this->z -= vec4f.z;
			this->w -= vec4f.w;
			return *this;
		}

		inline Vector4f& operator *=(const float& f) {
			this->x *= f;
			this->y *= f;
			this->z *= f;
			this->w *= f;
			return *this;
		}

		inline Vector4f& operator *=(const Vector4f& vec4f) {
			this->x *= vec4f.x;
			this->y *= vec4f.y;
			this->z *= vec4f.z;
			this->w *= vec4f.w;
			return *this;
		}

		inline Vector4f& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			this->z /= f;
			this->w /= f;
			return *this;
		}

		inline Vector4f& operator /=(const Vector4f& vec4f) {
			this->x /= vec4f.x;
			this->y /= vec4f.y;
			this->z /= vec4f.z;
			this->w /= vec4f.w;
			return *this;
		}

		inline Vector4f operator +(const float& f) const {
			return Vector4f(x + f, y + f, z + f, w + f);
		}

		inline Vector4f operator +(const Vector4f& vec4f) const {
			return Vector4f(x + vec4f.x, y + vec4f.y, z + vec4f.z, w + vec4f.w);
		}

		inline Vector4f operator -(const float& f) const {
			return Vector4f(x - f, y - f, z - f, w - f);
		}

		inline Vector4f operator -(const Vector4f& vec4f) const {
			return Vector4f(x - vec4f.x, y - vec4f.y, z - vec4f.z, w - vec4f.w);
		}

		inline Vector4f operator *(const float& f) const {
			return Vector4f(x * f, y * f, z * f, w * f);
		}

		inline Vector4f operator *(const Vector4f& vec4f) const {
			return Vector4f(x * vec4f.x, y * vec4f.y, z * vec4f.z, w * vec4f.w);
		}

		inline Vector4f operator /(const float& f) const {
			return Vector4f(x / f, y / f, z / f, w / f);
		}

		inline Vector4f operator /(const Vector4f& vec4f) const {
			return Vector4f(x / vec4f.x, y / vec4f.y, z / vec4f.z, w / vec4f.w);
		}

		Vector4f operator *(const Matrix4f& mat4f) const;
		Vector4f Mul(const Matrix4f& mat4f) const;
	};
} }