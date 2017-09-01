#include "ge.h"
#include "Quaternion.h"

namespace gebase { namespace math {

#define _VECTORMATH_SLERP_TOL 0.999f

	Quaternion::Quaternion()
		: x(0), y(0), z(0), w(1)
	{
	}

	Quaternion::Quaternion(const Quaternion& Quaternion)
	{
		x = Quaternion.x;
		y = Quaternion.y;
		z = Quaternion.z;
		w = Quaternion.w;
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	Quaternion::Quaternion(const Vector4f& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	Quaternion::Quaternion(float scalar)
	{
		x = scalar;
		y = scalar;
		z = scalar;
		w = scalar;
	}

	Quaternion::Quaternion(const Vector3f& xyz, float w)
	{
		this->setXYZ(xyz);
		this->w = w;
	}

	Quaternion Quaternion::Identity()
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4f Quaternion::toRotationMatrix() const
	{
		return math::Matrix4f::Rotation(*this);
	}

	Quaternion Quaternion::FromEulerAngles(const Vector3f& angles)
	{
		Quaternion pitch(Vector3f(1.0, 0.0, 0.0), angles.x);
		Quaternion yaw(Vector3f(0.0, 1.0, 0.0), angles.y);
		Quaternion roll(Vector3f(0.0, 0.0, 1.0), angles.z);
		return pitch * yaw * roll;
	}

	Quaternion& Quaternion::operator =(const Quaternion & Quaternion)
	{
		x = Quaternion.x;
		y = Quaternion.y;
		z = Quaternion.z;
		w = Quaternion.w;
		return *this;
	}

	Quaternion& Quaternion::setXYZ(const Vector3f & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	const Vector3f Quaternion::getXYZ() const
	{
		return Vector3f(x, y, z);
	}

	Quaternion& Quaternion::SetElem(int32 idx, float value)
	{
		*(&x + idx) = value;
		return *this;
	}

	float Quaternion::GetElem(int32 idx) const
	{
		return *(&x + idx);
	}

	float Quaternion::operator [](int32 idx) const
	{
		return *(&x + idx);
	}

	Vector3f Quaternion::GetAxis() const
	{
		float x = 1.0f - w * w;

		if (x < 0.0000001f)
			return Vector3f(1.0f, 0.0f, 0.0f);

		float x2 = x * x;
		return getXYZ() / x2;
	}

	Vector3f Quaternion::ToEulerAngles() const
	{
		return Vector3f(::atan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z), ::atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z), ::asin(2 * x * y + 2 * z * w));
	}

	const Quaternion Quaternion::operator+(const Quaternion& quaternion) const
	{
		return Quaternion(x + quaternion.x, y + quaternion.y, z + quaternion.z, w + quaternion.w);
	}

	const Quaternion Quaternion::operator-(const Quaternion& quaternion) const
	{
		return Quaternion(x - quaternion.x, y - quaternion.y, z - quaternion.z, w - quaternion.w);
	}

	const Quaternion Quaternion::operator*(float scalar) const
	{
		return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	const Quaternion Quaternion::operator/(float scalar) const
	{
		return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	const Quaternion Quaternion::operator-() const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	bool Quaternion::operator ==(const Quaternion & Quaternion) const
	{
		return (x == Quaternion.x) && (y == Quaternion.y) && (z == Quaternion.z) && (w == Quaternion.w);
	}

	bool Quaternion::operator !=(const Quaternion & Quaternion) const
	{
		return !(*this == Quaternion);
	}

	float Norm(const Quaternion& quaternion)
	{
		float result;
		result = (quaternion.x * quaternion.x);
		result = (result + (quaternion.y * quaternion.y));
		result = (result + (quaternion.z * quaternion.z));
		result = (result + (quaternion.w * quaternion.w));
		return result;
	}

	float Length(const Quaternion& quaternion)
	{
		return sqrt(Norm(quaternion));
	}

	const Quaternion Normalize(const Quaternion& quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(quaternion);
		lenInv = 1.0f / ::sqrt(lenSqr);
		return quaternion * lenInv;
	}

	const Quaternion NormalizeEst(const Quaternion& quaternion)
	{
		float lenSqr, lenInv;
		lenSqr = Norm(quaternion);
		lenInv = 1.0f / ::sqrt(lenSqr);
		return quaternion * lenInv;
	}

	const Quaternion Quaternion::Rotation(const Vector3f& unitVec0, const Vector3f& unitVec1)
	{
		float cosHalfAngleX2, recipCosHalfAngleX2;
		cosHalfAngleX2 = ::sqrt((2.0f * (1.0f + unitVec0.dot(unitVec1))));
		recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
		return Quaternion((unitVec0.cross(unitVec1) * recipCosHalfAngleX2), (cosHalfAngleX2 * 0.5f));
	}

	const Quaternion Quaternion::Rotation(float radians, const Vector3f & unitVec)
	{
		float angle = radians * 0.5f;
		return Quaternion((unitVec * ::sin(angle)), ::cos(angle));
	}

	const Quaternion Quaternion::operator*(const Quaternion& quat) const
	{
		return Normalize(Quaternion(
			(((w * quat.x) + (x * quat.w)) + (y * quat.z)) - (z * quat.y),
			(((w * quat.y) + (y * quat.w)) + (z * quat.x)) - (x * quat.z),
			(((w * quat.z) + (z * quat.w)) + (x * quat.y)) - (y * quat.x),
			(((w * quat.w) - (x * quat.x)) - (y * quat.y)) - (z * quat.z)
		));
	}

	Vector3f Quaternion::Rotate(const Quaternion& quat, const Vector3f& vec)
	{
		float tmpX, tmpY, tmpZ, tmpW;

		tmpX = (((quat.w * vec.x) + (quat.y * vec.z)) - (quat.z * vec.y));
		tmpY = (((quat.w * vec.y) + (quat.z * vec.x)) - (quat.x * vec.z));
		tmpZ = (((quat.w * vec.z) + (quat.x * vec.y)) - (quat.y * vec.x));
		tmpW = (((quat.x * vec.x) + (quat.y * vec.y)) + (quat.z * vec.z));

		return Vector3f(((((tmpW * quat.x) + (tmpX * quat.w)) - (tmpY * quat.z)) + (tmpZ * quat.y)),
						((((tmpW * quat.y) + (tmpY * quat.w)) - (tmpZ * quat.x)) + (tmpX * quat.z)),
						((((tmpW * quat.z) + (tmpZ * quat.w)) - (tmpX * quat.y)) + (tmpY * quat.x))
		);
	}

	Quaternion Quaternion::conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	const Quaternion Select(const Quaternion& quat0, const Quaternion& quat1, bool select1)
	{
		return Quaternion(select1 ? quat1.x : quat0.x, select1 ? quat1.y : quat0.y, select1 ? quat1.z : quat0.z, select1 ? quat1.w : quat0.w);
	}

	float Quaternion::dot(const Quaternion& other) const
	{
		float result = (x * other.x);
		result = (result + (y * other.y));
		result = (result + (z * other.z));
		result = (result + (w * other.w));
		return result;
	}

} }