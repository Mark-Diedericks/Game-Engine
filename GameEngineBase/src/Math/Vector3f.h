#pragma once

#include <math.h>

#include "Common.h"

namespace gebase { namespace math {

	class Vector2f;
	class Vector4f;
	class Matrix4f;

	class GE_API Vector3f {
	public:
		float x, y, z;

		inline Vector3f() : x(0), y(0), z(0) {}
		Vector3f(const float& ix, const float& iy, const float& iz);
		Vector3f(const Vector2f& vector);
		Vector3f(const Vector4f& vector);
		float getLength() const;
		float dot(const Vector3f& vec3f) const;
		Vector3f cross(const Vector3f& vec3f) const;
		Vector3f normalize();
		Vector3f rotate(const Vector3f& am_xis, const float& angle);
		Vector3f lerp(const Vector3f& vec3f, const float& factor) const;
		Vector3f refelect(const Vector3f& normal) const;

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
			this->z = f;
		}

		inline void operator =(const Vector3f& vec3f) {
			this->x = vec3f.x;
			this->y = vec3f.y;
			this->z = vec3f.z;
		}

		inline bool operator ==(const Vector3f& vec3f) const {
			return this->x == vec3f.x && this->y == vec3f.y && this->z == vec3f.z;
		}

		inline bool operator <(const Vector3f& vec3f) const {
			return this->x < vec3f.x && this->y < vec3f.y && this->z < vec3f.z;
		}

		inline bool operator >(const Vector3f& vec3f) const {
			return this->x > vec3f.x && this->y > vec3f.y && this->z > vec3f.z;
		}

		inline bool operator <=(const Vector3f& vec3f) const {
			return *this < vec3f || *this == vec3f;
		}

		inline bool operator >=(const Vector3f& vec3f) const {
			return *this > vec3f || *this == vec3f;
		}

		inline Vector3f& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			this->z += f;
			return *this;
		}

		inline Vector3f& operator +=(const Vector3f& vec3f) {
			this->x += vec3f.x;
			this->y += vec3f.y;
			this->z += vec3f.z;
			return *this;
		}

		inline Vector3f& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			this->z -= f;
			return *this;
		}

		inline Vector3f& operator -=(const Vector3f& vec3f) {
			this->x -= vec3f.x;
			this->y -= vec3f.y;
			this->z -= vec3f.z;
			return *this;
		}

		inline Vector3f& operator *=(const float& f) {
			this->x *= f;
			this->y *= f;
			this->z *= f;
			return *this;
		}

		inline Vector3f& operator *=(const Vector3f& vec3f) {
			this->x *= vec3f.x;
			this->y *= vec3f.y;
			this->z *= vec3f.z;
			return *this;
		}

		inline Vector3f& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			this->z /= f;
			return *this;
		}

		inline Vector3f& operator /=(const Vector3f& vec3f) {
			this->x /= vec3f.x;
			this->y /= vec3f.y;
			this->z /= vec3f.z;
			return *this;
		}

		inline Vector3f operator +(const float& f) const {
			return Vector3f(x + f, y + f, z + f);
		}

		inline Vector3f operator +(const Vector3f& vec3f) const {
			return Vector3f(x + vec3f.x, y + vec3f.y, z + vec3f.z);
		}

		inline Vector3f operator -(const float& f) const {
			return Vector3f(x - f, y - f, z - f);
		}

		inline Vector3f operator -(const Vector3f& vec3f) const {
			return Vector3f(x - vec3f.x, y - vec3f.y, z - vec3f.z);
		}

		inline Vector3f operator *(const float& f) const {
			return Vector3f(x * f, y * f, z * f);
		}

		inline Vector3f operator *(const Vector3f& vec3f) const {
			return Vector3f(x * vec3f.x, y * vec3f.y, z * vec3f.z);
		}

		inline Vector3f operator /(const float& f) const {
			return Vector3f(x / f, y / f, z / f);
		}

		inline Vector3f operator /(const Vector3f& vec3f) const {
			return Vector3f(x / vec3f.x, y / vec3f.y, z / vec3f.z);
		}

		Vector3f operator *(const Matrix4f& mat4f) const;
		Vector3f Mul(const Matrix4f& mat4f) const;
	};
} }
