#pragma once

#include <math.h>

#include "Common.h"
#include "Utils.h"

namespace gebase { namespace math {

	class Vector2f;
	class Vector4f;
	struct Matrix4f;

	class GE_API Vector3f {
	public:
		float x, y, z;

		inline Vector3f() : x(0), y(0), z(0) {}
		Vector3f(const float& ix, const float& iy, const float& iz);
		Vector3f(const Vector2f& vector);
		Vector3f(const Vector4f& vector);
		float getLength() const;
		float dot(const Vector3f& Vector3ff) const;
		Vector3f cross(const Vector3f& Vector3ff) const;
		Vector3f normalize();
		Vector3f rotate(const Vector3f& am_xis, const float& angle);
		Vector3f lerp(const Vector3f& Vector3ff, const float& factor) const;
		Vector3f refelect(const Vector3f& normal) const;

		inline Vector3f* Floor()
		{
			this->x = floor(x);
			this->y = floor(y);
			this->z = floor(z);
			return this;
		}

		inline Vector3f* Ceiling()
		{
			this->x = ceiling(x);
			this->y = ceiling(y);
			this->z = ceiling(z);
			return this;
		}

		inline Vector3f* Round()
		{
			this->x = round(x);
			this->y = round(y);
			this->z = round(z);
			return this;
		}

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
			this->z = f;
		}

		inline void operator =(const Vector3f& Vector3ff) {
			this->x = Vector3ff.x;
			this->y = Vector3ff.y;
			this->z = Vector3ff.z;
		}

		inline bool operator ==(const Vector3f& Vector3ff) const {
			return this->x == Vector3ff.x && this->y == Vector3ff.y && this->z == Vector3ff.z;
		}

		inline bool operator <(const Vector3f& Vector3ff) const {
			return this->x < Vector3ff.x && this->y < Vector3ff.y && this->z < Vector3ff.z;
		}

		inline bool operator >(const Vector3f& Vector3ff) const {
			return this->x > Vector3ff.x && this->y > Vector3ff.y && this->z > Vector3ff.z;
		}

		inline bool operator <=(const Vector3f& Vector3ff) const {
			return *this < Vector3ff || *this == Vector3ff;
		}

		inline bool operator >=(const Vector3f& Vector3ff) const {
			return *this > Vector3ff || *this == Vector3ff;
		}

		inline Vector3f& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			this->z += f;
			return *this;
		}

		inline Vector3f& operator +=(const Vector3f& Vector3ff) {
			this->x += Vector3ff.x;
			this->y += Vector3ff.y;
			this->z += Vector3ff.z;
			return *this;
		}

		inline Vector3f& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			this->z -= f;
			return *this;
		}

		inline Vector3f& operator -=(const Vector3f& Vector3ff) {
			this->x -= Vector3ff.x;
			this->y -= Vector3ff.y;
			this->z -= Vector3ff.z;
			return *this;
		}

		inline Vector3f& operator *=(const float& f) {
			this->x *= f;
			this->y *= f;
			this->z *= f;
			return *this;
		}

		inline Vector3f& operator *=(const Vector3f& Vector3ff) {
			this->x *= Vector3ff.x;
			this->y *= Vector3ff.y;
			this->z *= Vector3ff.z;
			return *this;
		}

		inline Vector3f& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			this->z /= f;
			return *this;
		}

		inline Vector3f& operator /=(const Vector3f& Vector3ff) {
			this->x /= Vector3ff.x;
			this->y /= Vector3ff.y;
			this->z /= Vector3ff.z;
			return *this;
		}

		inline Vector3f operator +(const float& f) const {
			return Vector3f(x + f, y + f, z + f);
		}

		inline Vector3f operator +(const Vector3f& Vector3ff) const {
			return Vector3f(x + Vector3ff.x, y + Vector3ff.y, z + Vector3ff.z);
		}

		inline Vector3f operator -(const float& f) const {
			return Vector3f(x - f, y - f, z - f);
		}

		inline Vector3f operator -(const Vector3f& Vector3ff) const {
			return Vector3f(x - Vector3ff.x, y - Vector3ff.y, z - Vector3ff.z);
		}

		inline Vector3f operator *(const float& f) const {
			return Vector3f(x * f, y * f, z * f);
		}

		inline Vector3f operator *(const Vector3f& Vector3ff) const {
			return Vector3f(x * Vector3ff.x, y * Vector3ff.y, z * Vector3ff.z);
		}

		inline Vector3f operator /(const float& f) const {
			return Vector3f(x / f, y / f, z / f);
		}

		inline Vector3f operator /(const Vector3f& Vector3ff) const {
			return Vector3f(x / Vector3ff.x, y / Vector3ff.y, z / Vector3ff.z);
		}

		Vector3f operator *(const Matrix4f& Matrix4ff) const;
		Vector3f Mul(const Matrix4f& Matrix4ff) const;
	};
} }
