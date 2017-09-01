#include "ge.h"
#include "Vector3f.h"
#include "Vector2f.h"
#include "Vector4f.h"
#include "Matrix4f.h"

namespace gebase { namespace math {
	
	Vector3f::Vector3f(const float& ix, const float& iy, const float& iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	Vector3f::Vector3f(const Vector2f& vector) : x(vector.x), y(vector.y), z(0)
	{

	}	
	
	Vector3f::Vector3f(const Vector4f& vector) : x(vector.x), y(vector.y), z(vector.z)
	{

	}

	float Vector3f::getLength() const {
		return (float)::sqrt(x * x + y * y + z * z);
	}

	float Vector3f::dot(const Vector3f& Vector3ff) const {
		return x * Vector3ff.x + y * Vector3ff.y + z * Vector3ff.z;
	}

	Vector3f Vector3f::cross(const Vector3f& Vector3ff) const {
		float m_x = y * Vector3ff.z - z * Vector3ff.y;
		float y = z * Vector3ff.x - m_x * Vector3ff.z;
		float z = m_x * Vector3ff.y - y * Vector3ff.x;
		return Vector3f(m_x, y, z);
	}

	Vector3f Vector3f::refelect(const Vector3f& normal) const {
		return *this - (normal * (this->dot(normal) * 2));
	}

	Vector3f Vector3f::rotate(const Vector3f& axis, const float& angle) {
		const float sinAngle = (float)::sin(-angle);
		const float cosAngle = (float)::cos(-angle);
		return this->cross(axis * sinAngle) + (*this * cosAngle) + axis * this->dot(axis * (1 - cosAngle));
	}

	Vector3f Vector3f::lerp(const Vector3f& Vector3ff, const float& factor) const {
		return ((Vector3ff - *this) * factor) - *this;
	}

	Vector3f Vector3f::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		z /= l;
		return (Vector3f)*this;
	}

	Vector3f Vector3f::operator *(const Matrix4f& Matrix4ff) const {
		return Vector3f(Matrix4ff.rows[0].x * x + Matrix4ff.rows[0].y * y + Matrix4ff.rows[0].z * z + Matrix4ff.rows[0].w, Matrix4ff.rows[1].x * x + Matrix4ff.rows[1].y * y + Matrix4ff.rows[1].z * z + Matrix4ff.rows[1].w, Matrix4ff.rows[2].x * x + Matrix4ff.rows[2].y * y + Matrix4ff.rows[2].z * z + Matrix4ff.rows[2].w);
	}

	Vector3f Vector3f::Mul(const Matrix4f& Matrix4ff) const {
		return Vector3f(Matrix4ff.rows[0].x * x + Matrix4ff.rows[0].y * y + Matrix4ff.rows[0].z * z + Matrix4ff.rows[0].w, Matrix4ff.rows[1].x * x + Matrix4ff.rows[1].y * y + Matrix4ff.rows[1].z * z + Matrix4ff.rows[1].w, Matrix4ff.rows[2].x * x + Matrix4ff.rows[2].y * y + Matrix4ff.rows[2].z * z + Matrix4ff.rows[2].w);
	}
} }