#pragma once

#include "Common.h"
#include "Matrix4f.h"
#include <cmath>

namespace gebase { namespace math {
	class GE_API Quaternion {
	private:
		float x, y, z, w;
	public:
		Quaternion(void);
		Quaternion(const float& ix, const float& iy, const float& iz, const float& iw);
		Quaternion(const Vector3f& axis, const float& angle);
		Quaternion(const Matrix4f& rot);
		Quaternion(const Vector3f& direction, const Vector3f& up) : Quaternion(*Matrix4f::initRotation(direction, up)) {}
		float getLength() const;
		float dot(const Quaternion& quat) const;
		Quaternion normalize();
		Quaternion conjugate();
		Quaternion nlerp(const Quaternion& dest, const float& lerpFactor, const bool& shortest) const;
		Quaternion slerp(const Quaternion& dest, const float& lerpFactor, const bool& shortest) const;
		Vector3f rotate(const Vector3f& vec3f);
		Matrix4f* toRotationMatrix();
		Vector3f& getRotateForward() const;
		Vector3f& getRotateUp() const;
		Vector3f& getRotateRight() const;
		Vector3f& getForward();
		Vector3f& getBack();
		Vector3f& getUp();
		Vector3f& getDown();
		Vector3f& getRight();
		Vector3f& getLeft();

		inline void operator =(const float& f) {
			this->x = f;
			this->y = f;
			this->z = f;
			this->w = f;
		}

		inline void operator =(const Quaternion& quat) {
			this->x = quat.x;
			this->y = quat.y;
			this->z = quat.z;
			this->w = quat.w;
		}

		inline bool operator ==(const Quaternion& quat) const {
			return this->x == quat.x && this->y == quat.y && this->z == quat.z && this->w == quat.w;
		}

		inline Quaternion& operator +=(const float& f) {
			this->x += f;
			this->y += f;
			this->z += f;
			this->w += f;
			return *this;
		}

		inline Quaternion& operator +=(const Quaternion& quat) {
			this->x += quat.x;
			this->y += quat.y;
			this->z += quat.z;
			this->w += quat.w;
			return *this;
		}

		inline Quaternion& operator -=(const float& f) {
			this->x -= f;
			this->y -= f;
			this->z -= f;
			this->w -= f;
			return *this;
		}

		inline Quaternion& operator -=(const Quaternion& quat) {
			this->x -= quat.x;
			this->y -= quat.y;
			this->z -= quat.z;
			this->w -= quat.w;
			return *this;
		}

		inline Quaternion& operator *=(const Vector3f& vec3f) {
			this->w = -x * vec3f.x - y * vec3f.y - z * vec3f.z;
			this->x = w * vec3f.x + y * vec3f.z - z * vec3f.y;
			this->y = w * vec3f.y + z * vec3f.x - x * vec3f.z;
			this->z = w * vec3f.z + x * vec3f.y - y * vec3f.x;
			return *this;
		}

		inline Quaternion& operator *=(const Quaternion& r) {
			this->w = w * r.w - x * r.x - y * r.y - z * r.z;
			this->x = x * r.w + w * r.x + y * r.z - z * r.y;
			this->y = y * r.w + w * r.y + z * r.x - x * r.z;
			this->z = z * r.w + w * r.z + x * r.y - y * r.x;
			return *this;
		}

		inline Quaternion& operator *=(const float& f) {
			Vector3f vec3f(f, f, f);
			*this *= vec3f;
			return *this;
		}

		inline Quaternion& operator /=(const float& f) {
			this->x /= f;
			this->y /= f;
			this->z /= f;
			this->w /= f;
			return *this;
		}

		inline Quaternion& operator /=(const Quaternion& quat) {
			this->x /= quat.x;
			this->y /= quat.y;
			this->z /= quat.z;
			this->w /= quat.w;
			return *this;
		}

		inline Quaternion& operator +(const float& f) const {
			Quaternion res(x + f, y + f, z + f, w + f);
			return res;
		}

		inline Quaternion& operator +(const Quaternion& quat) const {
			Quaternion res(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
			return res;
		}

		inline Quaternion& operator -(const float& f) const {
			Quaternion res(x - f, y - f, z - f, w - f);
			return res;
		}

		inline Quaternion& operator -(const Quaternion& quat) const {
			Quaternion res(x - quat.x, y - quat.y, z - quat.z, w - quat.w);
			return res;
		}

		inline Quaternion& operator *(const Vector3f& vec3f) const {
			float w_ = -x * vec3f.x - y * vec3f.y - z * vec3f.z;
			float x_ = w * vec3f.x + y * vec3f.z - z * vec3f.y;
			float y_ = w * vec3f.y + z * vec3f.x - x * vec3f.z;
			float z_ = w * vec3f.z + x * vec3f.y - y * vec3f.x;
			
			Quaternion res(x_, y_, z_, w_);
			return res;
		}

		inline Quaternion& operator *(const Quaternion& r) const {
			float w_ = w * r.w - x * r.x - y * r.y - z * r.z;
			float x_ = x * r.w + w * r.x + y * r.z - z * r.y;
			float y_ = y * r.w + w * r.y + z * r.x - x * r.z;
			float z_ = z * r.w + w * r.z + x * r.y - y * r.x;

			Quaternion res(x_, y_, z_, w_);
			return res;
		}

		inline Quaternion& operator *(const float& f) const {
			Vector3f vec3f(f, f, f);
			return ((*this) * vec3f);
		}

		inline Quaternion& operator /(const float& f) const {
			Quaternion res(x / f, y / f, z / f, w / f);
			return res;
		}

		inline Quaternion& operator /(const Quaternion& quat) const {
			Quaternion res(x / quat.x, y / quat.y, z / quat.z, w / quat.w);
			return res;
		}
	};
} }