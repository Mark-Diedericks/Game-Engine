#include "ge.h"
#include "Vector4f.h"
#include "Matrix4f.h"

namespace gebase { namespace math {
	Vector4f::Vector4f(const float& ix, const float& iy, const float& iz, const float& iw) {
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	Vector4f::Vector4f(const Vector3f& vector, const float& iw) {
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = iw;
	}

	float Vector4f::getLength() const {
		return (float)::sqrt(x * x + y * y + z * z + w * w);
	}

	float Vector4f::dot(const Vector4f& Vector4ff) const {
		return x * Vector4ff.x + y * Vector4ff.y + z * Vector4ff.z + w * Vector4ff.w;
	}

	Vector4f Vector4f::refelect(const Vector4f& normal) const {
		return *this - (normal * (this->dot(normal) * 2));
	}


	Vector4f Vector4f::lerp(const Vector4f& Vector4ff, const float& factor) const {
		return ((Vector4ff - *this) * factor) - *this;
	}

	Vector4f Vector4f::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		z /= l;
		w /= l;
		return (Vector4f)*this;
	}

	Vector4f Vector4f::operator *(const Matrix4f& Matrix4ff) const {
		return Vector4f(Matrix4ff.rows[0].x * x + Matrix4ff.rows[0].y * y + Matrix4ff.rows[0].z * z + Matrix4ff.rows[0].w * w, Matrix4ff.rows[1].x * x + Matrix4ff.rows[1].y * y + Matrix4ff.rows[1].z * z + Matrix4ff.rows[1].w * w, Matrix4ff.rows[2].x * x + Matrix4ff.rows[2].y * y + Matrix4ff.rows[2].z * z + Matrix4ff.rows[2].w * w, Matrix4ff.rows[3].x * x + Matrix4ff.rows[3].y * y + Matrix4ff.rows[3].z * z + Matrix4ff.rows[3].w * w);
	}

	Vector4f Vector4f::Mul(const Matrix4f& Matrix4ff) const {
		return Vector4f(Matrix4ff.rows[0].x * x + Matrix4ff.rows[0].y * y + Matrix4ff.rows[0].z * z + Matrix4ff.rows[0].w * w, Matrix4ff.rows[1].x * x + Matrix4ff.rows[1].y * y + Matrix4ff.rows[1].z * z + Matrix4ff.rows[1].w * w, Matrix4ff.rows[2].x * x + Matrix4ff.rows[2].y * y + Matrix4ff.rows[2].z * z + Matrix4ff.rows[2].w * w, Matrix4ff.rows[3].x * x + Matrix4ff.rows[3].y * y + Matrix4ff.rows[3].z * z + Matrix4ff.rows[3].w * w);
	}
} }