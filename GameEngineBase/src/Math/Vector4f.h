#pragma once

#include <math.h>

#include "Common.h"
#include "Vector3f.h"
#include "Utils.h"

namespace gebase { namespace math {

	struct Matrix4f;

	class GE_API Vector4f {
	public:
		float x, y, z, w;

		inline Vector4f() : x(0), y(0), z(0), w(0) {}
		Vector4f(const float& ix, const float& iy, const float& iz, const float& iw);
		Vector4f(const Vector3f& vector, const float& iw);
		float getLength() const;
		float dot(const Vector4f& Vector4ff) const;
		Vector4f normalize();
		Vector4f lerp(const Vector4f& Vector4ff, const float& factor) const;
		Vector4f refelect(const Vector4f& normal) const;

		inline Vector4f* Floor()
		{
			this->x = floor(x);
			this->y = floor(y);
			this->z = floor(z);
			this->w = floor(w);
			return this;
		}

		inline Vector4f* Ceiling()
		{
			this->x = ceiling(x);
			this->y = ceiling(y);
			this->z = ceiling(z);
			this->w = ceiling(w);
			return this;
		}

		inline Vector4f* Round()
		{
			this->x = round(x);
			this->y = round(y);
			this->z = round(z);
			this->w = round(w);
			return this;
		}

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
			this->z = f;
			this->w = f;
		}

		inline void operator =(const Vector4f& Vector4ff) {
			this->x = Vector4ff.x;
			this->y = Vector4ff.y;
			this->z = Vector4ff.z;
			this->w = Vector4ff.w;
		}

		inline bool operator ==(const Vector4f& Vector4ff) const {
			return this->x == Vector4ff.x && this->y == Vector4ff.y && this->z == Vector4ff.z && this->w == Vector4ff.w;
		}

		inline Vector4f& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			this->z += f;
			this->w += f;
			return *this;
		}

		inline Vector4f& operator +=(const Vector4f& Vector4ff) {
			this->x += Vector4ff.x;
			this->y += Vector4ff.y;
			this->z += Vector4ff.z;
			this->w += Vector4ff.w;
			return *this;
		}

		inline Vector4f& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			this->z -= f;
			this->w -= f;
			return *this;
		}

		inline Vector4f& operator -=(const Vector4f& Vector4ff) {
			this->x -= Vector4ff.x;
			this->y -= Vector4ff.y;
			this->z -= Vector4ff.z;
			this->w -= Vector4ff.w;
			return *this;
		}

		inline Vector4f& operator *=(const float& f) {
			this->x *= f;
			this->y *= f;
			this->z *= f;
			this->w *= f;
			return *this;
		}

		inline Vector4f& operator *=(const Vector4f& Vector4ff) {
			this->x *= Vector4ff.x;
			this->y *= Vector4ff.y;
			this->z *= Vector4ff.z;
			this->w *= Vector4ff.w;
			return *this;
		}

		inline Vector4f& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			this->z /= f;
			this->w /= f;
			return *this;
		}

		inline Vector4f& operator /=(const Vector4f& Vector4ff) {
			this->x /= Vector4ff.x;
			this->y /= Vector4ff.y;
			this->z /= Vector4ff.z;
			this->w /= Vector4ff.w;
			return *this;
		}

		inline Vector4f operator +(const float& f) const {
			return Vector4f(x + f, y + f, z + f, w + f);
		}

		inline Vector4f operator +(const Vector4f& Vector4ff) const {
			return Vector4f(x + Vector4ff.x, y + Vector4ff.y, z + Vector4ff.z, w + Vector4ff.w);
		}

		inline Vector4f operator -(const float& f) const {
			return Vector4f(x - f, y - f, z - f, w - f);
		}

		inline Vector4f operator -(const Vector4f& Vector4ff) const {
			return Vector4f(x - Vector4ff.x, y - Vector4ff.y, z - Vector4ff.z, w - Vector4ff.w);
		}

		inline Vector4f operator *(const float& f) const {
			return Vector4f(x * f, y * f, z * f, w * f);
		}

		inline Vector4f operator *(const Vector4f& Vector4ff) const {
			return Vector4f(x * Vector4ff.x, y * Vector4ff.y, z * Vector4ff.z, w * Vector4ff.w);
		}

		inline Vector4f operator /(const float& f) const {
			return Vector4f(x / f, y / f, z / f, w / f);
		}

		inline Vector4f operator /(const Vector4f& Vector4ff) const {
			return Vector4f(x / Vector4ff.x, y / Vector4ff.y, z / Vector4ff.z, w / Vector4ff.w);
		}

		Vector4f operator *(const Matrix4f& Matrix4ff) const;
		Vector4f Mul(const Matrix4f& Matrix4ff) const;
	};
} }