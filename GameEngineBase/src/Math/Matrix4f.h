#pragma once

#include <math.h>

#include "Common.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Utils.h"

namespace gebase { namespace math {

	class Quaternion;

	class GE_API Matrix4f {
	public:
		union 
		{
			float m[4 * 4];
			Vector4f rows[4];
		};
	public:
		Matrix4f();
		Matrix4f(const float elements[]);
		Matrix4f(float* elements);
		Matrix4f(float diagonal);
		static Matrix4f initIdentity();
		static Matrix4f initDiagonal(const float& diagonal);
		static Matrix4f initPerspective(const float& fov, const float& aspectRatio, const float& zNear, const float& zFar);
		static Matrix4f initOrthographic(const float& left, const float& right, const float& top, const float& bottom, const float& near, const float& far);
		static Matrix4f initTranslation(const float& x, const float& y, const float& z);
		static inline Matrix4f initTranslation(const Vector3f& position) { return Matrix4f::initTranslation(position.x, position.y, position.z); }
		static Matrix4f initRotation(const Quaternion& quat);
		static Matrix4f initScale(const float& m_x, const float& y, const float& z);
		Matrix4f inverse() const;

		inline void operator =(const Matrix4f& mat4f) {
			for (int i = 0; i < 16; i++)
				m[i] = mat4f.m[i];
		}

		inline bool operator ==(const Matrix4f& mat4f) const {
			for (int i = 0; i < 16; i++)
				if (m[i] != mat4f.m[i])
					return false;

			return true;
		}

		inline Matrix4f& operator +=(const Matrix4f& mat4f) {
			for (int i = 0; i < 16; i++)
				m[i] += mat4f.m[i];

			return *this;
		}

		inline Matrix4f& operator -=(const Matrix4f& mat4f) {
			for (int i = 0; i < 16; i++)
				m[i] -= mat4f.m[i];

			return *this;
		}

		inline Matrix4f& operator *=(const Matrix4f& mat4f) {
			float data[16];
			for (int32 row = 0; row < 4; row++)
			{
				for (int32 col = 0; col < 4; col++)
				{
					float sum = 0.0f;
					for (int32 e = 0; e < 4; e++)
					{
						sum += m[e + row * 4] * mat4f.m[col + e * 4];
					}
					data[col + row * 4] = sum;
				}
			}
			memcpy(m, data, 4 * 4 * sizeof(float));
			return *this;
		}

		inline Matrix4f operator +(const Matrix4f& mat4f) const {
			return Matrix4f(m) += mat4f;
		}

		inline Matrix4f operator -(const Matrix4f& mat4f) const {
			return Matrix4f(m) -= mat4f;
		}

		inline Matrix4f operator *(const Matrix4f& mat4f) const {
			return Matrix4f(m) *= mat4f;
		}

		inline Vector3f operator *(const Vector3f& vec3f) const {
			return vec3f.Mul(*this);
		}

		inline Vector4f operator *(const Vector4f& vec4f) const {
			return vec4f.Mul(*this);
		}
	};
} }