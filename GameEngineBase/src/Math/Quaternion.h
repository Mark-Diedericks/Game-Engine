#pragma once

#include <math.h>

#include "Common.h"
#include "Matrix4f.h"

namespace gebase { namespace math {

	struct GE_API Quaternion
	{
		float x, y, z, w;

		Quaternion();
		Quaternion(const Quaternion& quaternion);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Vector3f& xyz, float w);
		Quaternion(const Vector4f& vec);
		Quaternion(float scalar);

		Quaternion& operator=(const Quaternion& quat);

		Quaternion& SetXYZ(const Vector3f& vec);
		const Vector3f GetXYZ() const;

		Quaternion& SetElem(int32 idx, float value);
		float GetElem(int32 idx) const;
		Vector3f GetAxis() const;
		Vector3f ToEulerAngles() const;

		const Quaternion operator+(const Quaternion& Quaternion) const;
		const Quaternion operator-(const Quaternion& Quaternion) const;
		const Quaternion operator*(const Quaternion& Quaternion) const;
		const Quaternion operator*(float scalar) const;
		const Quaternion operator/(float scalar) const;
		float operator[](int32 idx) const;

		Quaternion& operator+=(const Quaternion& Quaternion)
		{
			*this = *this + Quaternion;
			return *this;
		}

		Quaternion& operator-=(const Quaternion& Quaternion)
		{
			*this = *this - Quaternion;
			return *this;
		}

		Quaternion& operator*=(const Quaternion& Quaternion)
		{
			*this = *this * Quaternion;
			return *this;
		}

		Quaternion& operator*=(float scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		Quaternion& operator/=(float scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		const Quaternion operator-() const;
		bool operator==(const Quaternion& quaternion) const;
		bool operator!=(const Quaternion& quaternion) const;
		static Quaternion Identity();
		static Quaternion FromEulerAngles(const Vector3f& angles);

		static Vector3f Rotate(const Quaternion & quat, const Vector3f & vec);

		static const Quaternion Rotation(const Vector3f& unitVec0, const Vector3f& unitVec1);
		static const Quaternion Rotation(float radians, const Vector3f& unitVec);

		static const Quaternion RotationX(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(sin(angle), 0.0f, 0.0f, cos(angle));
		}

		static const Quaternion RotationY(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(0.0f, sin(angle), 0.0f, cos(angle));
		}

		static const Quaternion RotationZ(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(0.0f, 0.0f, sin(angle), cos(angle));
		}

		float Dot(const Quaternion& other) const;
		Quaternion Conjugate() const;

	};

} }