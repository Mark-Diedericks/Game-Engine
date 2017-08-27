#pragma once

#include <math.h>

#include "Common.h"

namespace gebase { namespace math {
	
	class Vector3f;

	class GE_API Vector2f {
	public:
		float x, y;
	public:
		inline Vector2f() : x(0), y(0) {}
		Vector2f(const float& ix, const float& iy);
		Vector2f(const Vector3f& vector);
		float getLength() const;
		float dot(const Vector2f& vec2f) const;
		float cross(const Vector2f& vec2f) const;
		Vector2f normalize();
		Vector2f lerp(const Vector2f& vec2f, const float& factor) const;
		Vector2f refelect(const Vector2f& normal) const;

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
		}

		inline void operator =(const Vector2f& vec2f) {
			this->x = vec2f.x;
			this->y = vec2f.y;
		}

		inline bool operator ==(const Vector2f& vec2f) const {
			return this->x == vec2f.x && this->y == vec2f.y;
		}

		inline bool operator <(const Vector2f& vec2f) const {
			return this->x < vec2f.x && this->y < vec2f.y;
		}

		inline bool operator >(const Vector2f& vec2f) const {
			return this->x > vec2f.x && this->y > vec2f.y;
		}

		inline bool operator <=(const Vector2f& vec2f) const {
			return *this < vec2f || *this == vec2f;
		}

		inline bool operator >=(const Vector2f& vec2f) const {
			return *this > vec2f || *this == vec2f;
		}

		inline Vector2f& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			return *this;
		}

		inline Vector2f& operator +=(const Vector2f& vec2f) {
			this->x += vec2f.x;
			this->y += vec2f.y;
			return *this;
		}

		inline Vector2f& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			return *this;
		}

		inline Vector2f& operator -=(const Vector2f& vec2f) {
			this->x -= vec2f.x;
			this->y -= vec2f.y;
			return *this;
		}

		inline Vector2f& operator *=(const float& f) {
			this->x *= f;
			this->y *= f;
			return *this;
		}

		inline Vector2f& operator *=(const Vector2f& vec2f) {
			this->x *= vec2f.x;
			this->y *= vec2f.y;
			return *this;
		}

		inline Vector2f& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			return *this;
		}

		inline Vector2f& operator /=(const Vector2f& vec2f) {
			this->x /= vec2f.x;
			this->y /= vec2f.y;
			return *this;
		}

		inline Vector2f& operator +(const float& f) const {
			Vector2f res(x + f, y + f);
			return res;
		}

		inline Vector2f& operator +(const Vector2f& vec2f) const {
			Vector2f res(x + vec2f.x, y + vec2f.y);
			return res;
		}

		inline Vector2f& operator -(const float& f) const {
			Vector2f res(x - f, y - f);
			return res;
		}

		inline Vector2f& operator -(const Vector2f& vec2f) const {
			Vector2f res(x - vec2f.x, y - vec2f.y);
			return res;
		}

		inline Vector2f& operator *(const float& f) const {
			Vector2f res(x * f, y * f);
			return res;
		}

		inline Vector2f& operator *(const Vector2f& vec2f) const {
			Vector2f res(x * vec2f.x, y * vec2f.y);
			return res;
		}

		inline Vector2f& operator /(const float& f) const {
			Vector2f res(x / f, y / f);
			return res;
		}

		inline Vector2f& operator /(const Vector2f& vec2f) const {
			Vector2f res(x / vec2f.x, y / vec2f.y);
			return res;
		}
	};
} }