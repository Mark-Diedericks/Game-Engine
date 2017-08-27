#pragma once

#include <math.h>

#include "Common.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Utils.h"

namespace gebase { namespace math {
	class GE_API Matrix4f {
	public:
		union 
		{
			float m[4 * 4];
			Vector4f rows[4];
		};
	public:
		Matrix4f(void);
		Matrix4f(const float data[]);
		static Matrix4f& initIdentity();
		static Matrix4f& initDiagonal(const float& diagonal);
		static Matrix4f& initPerspective(const float& fov, const float& aspectRatio, const float& zNear, const float& zFar);
		static Matrix4f& initOrthographic(const float& left, const float& right, const float& top, const float& bottom, const float& near, const float& far);
		static Matrix4f& initTranslation(const float& m_x, const float& y, const float& z);
		static inline Matrix4f& initTranslation(const Vector3f& position) { return Matrix4f::initTranslation(position.x, position.y, position.z); }
		static Matrix4f& initRotation(const float& m_x, const float& y, const float& z);
		static Matrix4f& initRotation(const Vector3f& forward, const Vector3f& up);
		static Matrix4f& initRotation(const Vector3f& forward, const Vector3f& up, const Vector3f& right);
		static Matrix4f& initScale(const float& m_x, const float& y, const float& z);
		float get(const int& row, const int& column) const;
		void set(const int& row, const int& column, const float& value);
		Vector3f transform(const Vector3f& vec3f);
		Matrix4f scale(const float& scale);
		Matrix4f inverse() const;
		double determinant() const;

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
			for (int m_x = 0; m_x < 4; m_x++)
				for (int y = 0; y < 4; y++)
					this->set(m_x, y, get(m_x, 0) * (mat4f.get(0, y)) + get(m_x, 1) * (mat4f.get(1, y)) + get(m_x, 2) * (mat4f.get(2, y)) + get(m_x, 3) * (mat4f.get(3, y)));
			return *this;
		}

		inline Matrix4f& operator +(const Matrix4f& mat4f) const {
			return Matrix4f(m) += mat4f;
		}

		inline Matrix4f& operator -(const Matrix4f& mat4f) const {
			return Matrix4f(m) -= mat4f;
		}

		inline Matrix4f& operator *(const Matrix4f& mat4f) const {
			return Matrix4f(m) *= mat4f;
		}

		inline Vector3f& operator *(const Vector3f& vec3f) const {
			return vec3f.Mul(*this);
		}

		inline Vector4f& operator *(const Vector4f& vec4f) const {
			return vec4f.Mul(*this);
		}
	};
} }