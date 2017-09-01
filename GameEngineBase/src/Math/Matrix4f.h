#pragma once

#include <math.h>

#include "Common.h"
#include "CustomString.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Utils.h"

namespace gebase { namespace math {
	struct Quaternion;

	struct GE_API Matrix4f
	{
		union
		{
			// [row + col * 4]
			float elements[4 * 4];
			Vector4f rows[4];
		};

		Matrix4f();
		Matrix4f(float diagonal);
		Matrix4f(float* elements);
		Matrix4f(const Vector4f& row0, const Vector4f& row1, const Vector4f& row2, const Vector4f& row3);

		static Matrix4f Identity();

		Matrix4f& Multiply(const Matrix4f& other);
		friend GE_API Matrix4f operator*(Matrix4f left, const Matrix4f& right);
		Matrix4f& operator*=(const Matrix4f& other);

		Vector3f Multiply(const Vector3f& other) const;
		friend GE_API Vector3f operator*(const Matrix4f& left, const Vector3f& right);

		Vector4f Multiply(const Vector4f& other) const;
		friend GE_API Vector4f operator*(const Matrix4f& left, const Vector4f& right);

		inline void operator=(const Matrix4f& other) { memcpy(this->elements, other.elements, 4 * 4 * sizeof(float)); }

		Matrix4f& Invert();

		Vector4f GetColumn(int index) const;
		void SetColumn(uint index, const Vector4f& column);
		inline Vector3f GetPosition() const { return Vector3f(GetColumn(3)); }
		inline void SetPosition(const Vector3f& position) { SetColumn(3, Vector4f(position, 1.0f)); }

		static Matrix4f Orthographic(float left, float right, float bottom, float top, float near, float far);
		static Matrix4f Perspective(float fov, float aspectRatio, float near, float far);
		static Matrix4f LookAt(const Vector3f& camera, const Vector3f& object, const Vector3f& up);

		static Matrix4f Translate(const Vector3f& translation);
		static inline Matrix4f Translation(const Vector3f& translation) { return Translate(translation); }
		static inline Matrix4f Translation(const float& x, const float& y, const float& z) { return Translate(Vector3f(x, y, z)); }

		static Matrix4f Rotate(float angle, const Vector3f& axis);
		static inline Matrix4f Rotation(const Vector3f& axis, float angle) { return Rotate(angle, axis); }

		static Matrix4f Rotate(const Quaternion& quat);
		static inline Matrix4f Rotation(const Quaternion& quat) { return Rotate(quat); }

		static Matrix4f Scale(const Vector3f& scale);
		static inline Matrix4f Scale(const float& x, const float& y, const float& z) { return Scale(Vector3f(x, y, z)); }

		static Matrix4f Invert(const Matrix4f& matrix);

		static Matrix4f Transpose(const Matrix4f& matrix);

		String ToString() const;
	};
} }