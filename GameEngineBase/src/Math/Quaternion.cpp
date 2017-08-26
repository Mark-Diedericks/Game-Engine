#include "Pch.h"
#include "Quaternion.h"

namespace gebase { namespace math {
	Quaternion::Quaternion(void)
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	Quaternion::Quaternion(const float& ix, const float& iy, const float& iz, const float& iw) {
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	Quaternion::Quaternion(const Vector3f& am_xis, const float& angle) {
		float sinHalfAngle = (float)std::sin(angle / 2.0f);
		float cosHalfAngle = (float)std::cos(angle / 2.0f);

		this->x = am_xis.x * sinHalfAngle;
		this->y = am_xis.y * sinHalfAngle;
		this->z = am_xis.z * sinHalfAngle;
		this->w = cosHalfAngle;
	}

	Quaternion::Quaternion(const Matrix4f& rot) {
		float trace = rot.get(0, 0) + rot.get(1, 1) + rot.get(2, 2);

		if (trace > 0)
		{
			float s = 0.5f / (float)std::sqrt(trace + 1.0f);
			w = 0.25f / s;
			x = (rot.get(1, 2) - rot.get(2, 1)) * s;
			y = (rot.get(2, 0) - rot.get(0, 2)) * s;
			z = (rot.get(0, 1) - rot.get(1, 0)) * s;
		}
		else
		{
			if (rot.get(0, 0) > rot.get(1, 1) && rot.get(0, 0) > rot.get(2, 2))
			{
				float s = 2.0f * (float)std::sqrt(1.0f + rot.get(0, 0) - rot.get(1, 1) - rot.get(2, 2));
				w = (rot.get(1, 2) - rot.get(2, 1)) / s;
				x = 0.25f * s;
				y = (rot.get(1, 0) + rot.get(0, 1)) / s;
				z = (rot.get(2, 0) + rot.get(0, 2)) / s;
			}
			else if (rot.get(1, 1) > rot.get(2, 2))
			{
				float s = 2.0f * (float)std::sqrt(1.0f + rot.get(1, 1) - rot.get(0, 0) - rot.get(2, 2));
				w = (rot.get(2, 0) - rot.get(0, 2)) / s;
				x = (rot.get(1, 0) + rot.get(0, 1)) / s;
				y = 0.25f * s;
				z = (rot.get(2, 1) + rot.get(1, 2)) / s;
			}
			else
			{
				float s = 2.0f * (float)std::sqrt(1.0f + rot.get(2, 2) - rot.get(0, 0) - rot.get(1, 1));
				w = (rot.get(0, 1) - rot.get(1, 0)) / s;
				x = (rot.get(2, 0) + rot.get(0, 2)) / s;
				y = (rot.get(1, 2) + rot.get(2, 1)) / s;
				z = 0.25f * s;
			}
		}

		float length = (float)std::sqrt(x*x + y*y + z*z + w*w);
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	Vector3f Quaternion::rotate(const Vector3f& vec3f) {
		Quaternion conjugate = this->conjugate();

		Quaternion w = ((*this * vec3f) * conjugate);

		Vector3f res(w.x, w.y, w.z);

		return res;
	}

	float Quaternion::getLength() const {
		return (float)std::sqrt(x * x + y * y + z * z + w * w);
	}

	float Quaternion::dot(const Quaternion& quat) const {
		return x * quat.x + y * quat.y + z * quat.z + w * quat.w;
	}

	Quaternion Quaternion::normalize() {
		float l = getLength();
		x /= l;
		y /= l;
		z /= l;
		w /= l;
		return (Quaternion)*this;
	}

	Quaternion Quaternion::conjugate() {
		Quaternion res(-x, -y, -z, w);
		return res;
	}

	Quaternion Quaternion::nlerp(const Quaternion& dest, const float& lerpFactor, const bool& shortest) const {
		Quaternion correctedDest = dest;

		if (shortest && this->dot(dest) < 0)
			correctedDest = Quaternion(-dest.x, -dest.y, -dest.z, -dest.w);

		return (((correctedDest - *this) * lerpFactor) + *this).normalize();
	}

	Quaternion Quaternion::slerp(const Quaternion& dest, const float& lerpFactor, const bool& shortest) const {
		const float EPSILON = 1e3f;

		float cos = this->dot(dest);
		Quaternion correctedDest = dest;

		if (shortest && cos < 0)
		{
			cos = -cos;
			correctedDest = Quaternion(-dest.x, -dest.y, -dest.z, -dest.w);
		}

		if (std::abs(cos) >= 1 - EPSILON)
			return nlerp(correctedDest, lerpFactor, false);

		float sin = (float)std::sqrt(1.0f - cos * cos);
		float angle = (float)std::atan2(sin, cos);
		float invSin = 1.0f / sin;

		float srcFactor = (float)std::sin((1.0f - lerpFactor) * angle) * invSin;
		float destFactor = (float)std::sin((lerpFactor)* angle) * invSin;

		return (*this * srcFactor) + (correctedDest * destFactor);
	}

	Matrix4f* Quaternion::toRotationMatrix() {
		Matrix4f* res = Matrix4f::initRotation(getRotateForward(), getRotateUp(), getRotateRight());
		return res;
	}

	Vector3f& Quaternion::getRotateForward() const {
		Vector3f res(2.0f * (x*z - w*y), 2.0f * (y*z + w*x), 1.0f - 2.0f * (x*x + y*y));
		return res;
	}

	Vector3f& Quaternion::getRotateUp() const {
		Vector3f res(2.0f * (x*y + w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z - w*x));
		return res;
	}

	Vector3f& Quaternion::getRotateRight() const {
		Vector3f res(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y - w*z), 2.0f * (x*z + w*y));
		return res;
	}

	Vector3f& Quaternion::getForward() {
		Vector3f res(0, 0, 1);
		this->rotate(res);
		return res;
	}

	Vector3f& Quaternion::getBack() {
		Vector3f res(0, 0, -1);
		this->rotate(res);
		return res;
	}

	Vector3f& Quaternion::getUp() {
		Vector3f res(0, 1, 0);
		this->rotate(res);
		return res;
	}

	Vector3f& Quaternion::getDown() {
		Vector3f res(0, -1, 0);
		this->rotate(res);
		return res;
	}

	Vector3f& Quaternion::getRight() {
		Vector3f res(1, 0, 0);
		this->rotate(res);
		return res;
	}

	Vector3f& Quaternion::getLeft() {
		Vector3f res(-1, 0, 0);
		this->rotate(res);
		return res;
	}
} }