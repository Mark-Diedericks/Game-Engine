#include "ge.h"
#include "Quaternion.h"

namespace gebase {
	namespace math {
		Quaternion::Quaternion(void)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 1.0f;
		}

		Quaternion::Quaternion(const float& ix, const float& iy, const float& iz, const float& iw) {
			x = ix;
			y = iy;
			z = iz;
			w = iw;
		}

		Quaternion::Quaternion(const Vector3f& axis, const float& angle) {
			float sinHalfAngle = (float)::sin(angle / 2.0f);
			float cosHalfAngle = (float)::cos(angle / 2.0f);

			this->x = axis.x * sinHalfAngle;
			this->y = axis.y * sinHalfAngle;
			this->z = axis.z * sinHalfAngle;
			this->w = cosHalfAngle;
		}

		Vector3f Quaternion::ToEulerAngles() const
		{
			return Vector3f(::atan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z), ::atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z), ::asin(2 * x * y + 2 * z * w));
		}

		Quaternion Quaternion::FromEulerAngles(const Vector3f& angles)
		{
			Quaternion pitch(Vector3f(1.0, 0.0, 0.0), angles.x);
			Quaternion yaw(Vector3f(0.0, 1.0, 0.0), angles.y);
			Quaternion roll(Vector3f(0.0, 0.0, 1.0), angles.z);
			return pitch * yaw * roll;
		}

		Quaternion Quaternion::FromEulerAngles(float pitch, float roll, float yaw)
		{
			Quaternion q;

			double cy = cos(yaw * 0.5);
			double sy = sin(yaw * 0.5);
			double cr = cos(roll * 0.5);
			double sr = sin(roll * 0.5);
			double cp = cos(pitch * 0.5);
			double sp = sin(pitch * 0.5);

			q.w = cy * cr * cp + sy * sr * sp;
			q.x = cy * sr * cp - sy * cr * sp;
			q.y = cy * cr * sp + sy * sr * cp;
			q.z = sy * cr * cp - cy * sr * sp;
			return q;
		}

		Vector3f Quaternion::rotate(const Vector3f& vec3f) const {
			Quaternion conjugate = this->conjugate();

			Quaternion w = ((*this * vec3f) * conjugate);

			Vector3f res(w.x, w.y, w.z);

			return res;
		}

		float Quaternion::getLength() const {
			return (float)::sqrt(x * x + y * y + z * z + w * w);
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

		Quaternion Quaternion::conjugate() const {
			return Quaternion(-x, -y, -z, w);
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

			if (::abs(cos) >= 1 - EPSILON)
				return nlerp(correctedDest, lerpFactor, false);

			float sin = (float)::sqrt(1.0f - cos * cos);
			float angle = (float)::atan2(sin, cos);
			float invSin = 1.0f / sin;

			float srcFactor = (float)::sin((1.0f - lerpFactor) * angle) * invSin;
			float destFactor = (float)::sin((lerpFactor)* angle) * invSin;

			return (*this * srcFactor) + (correctedDest * destFactor);
		}

		Matrix4f Quaternion::toRotationMatrix() {
			return Matrix4f::Rotation(*this);
		}

		Vector3f Quaternion::getRotateForward() const {
			Vector3f res(2.0f * (x*z - w*y), 2.0f * (y*z + w*x), 1.0f - 2.0f * (x*x + y*y));
			return res;
		}

		Vector3f Quaternion::getRotateUp() const {
			Vector3f res(2.0f * (x*y + w*z), 1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z - w*x));
			return res;
		}

		Vector3f Quaternion::getRotateRight() const {
			Vector3f res(1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y - w*z), 2.0f * (x*z + w*y));
			return res;
		}

		Vector3f Quaternion::getForward() {
			Vector3f res(0, 0, 1);
			this->rotate(res);
			return res;
		}

		Vector3f Quaternion::getBack() {
			Vector3f res(0, 0, -1);
			this->rotate(res);
			return res;
		}

		Vector3f Quaternion::getUp() {
			Vector3f res(0, 1, 0);
			this->rotate(res);
			return res;
		}

		Vector3f Quaternion::getDown() {
			Vector3f res(0, -1, 0);
			this->rotate(res);
			return res;
		}

		Vector3f Quaternion::getRight() {
			Vector3f res(1, 0, 0);
			this->rotate(res);
			return res;
		}

		Vector3f Quaternion::getLeft() {
			Vector3f res(-1, 0, 0);
			this->rotate(res);
			return res;
		}
	}
}