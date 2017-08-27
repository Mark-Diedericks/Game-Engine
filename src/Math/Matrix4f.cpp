#include "ge.h"
#include "Matrix4f.h"

namespace gebase { namespace math {

	Matrix4f::Matrix4f(void) {
		for (int i = 0; i < 16; i++)
			m[i] = 0;
	}

	Matrix4f::Matrix4f(const float data[]) {
		for (int i = 0; i < 16; i++)
			m[i] = data[i];
	}

	Matrix4f* Matrix4f::initIdentity() {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		m[0] = 1; m[4] = 0;	m[8] = 0;   m[12] = 0;
		m[1] = 0; m[5] = 1;	m[9] = 0;   m[13] = 0;
		m[2] = 0; m[6] = 0;	m[10] = 1;  m[14] = 0;
		m[3] = 0; m[7] = 0;	m[11] = 0;  m[15] = 1;
		return mat;
	}

	Matrix4f* Matrix4f::initDiagonal(const float& diagonal) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		m[0] = diagonal; m[4] = 0;			m[8] = 0;		   m[12] = 0;
		m[1] = 0;		 m[5] = diagonal;	m[9] = 0;		   m[13] = 0;
		m[2] = 0;		 m[6] = 0;			m[10] = diagonal;  m[14] = 0;
		m[3] = 0;		 m[7] = 0;			m[11] = 0;		   m[15] = diagonal;
		return mat;
	}

	Matrix4f* Matrix4f::initPerspective(const float& fov, const float& aspectRatio, const float& zNear, const float& zFar) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		float tanHalfFOV = (float)::tan(fov / 2.0f);
		float zRange = zNear - zFar;

		m[0] = 1.0f / (tanHalfFOV * aspectRatio); m[4] = 0;					m[8] = 0;							m[12] = 0;
		m[1] = 0;								  m[5] = 1.0f / tanHalfFOV;	m[9] = 0;							m[13] = 0;
		m[2] = 0;								  m[6] = 0;					m[10] = (-zNear - zFar) / zRange;	m[14] = 2 * zFar * zNear / zRange;
		m[3] = 0;								  m[7] = 0;					m[11] = 1;							m[15] = 0;

		return mat;
	}

	Matrix4f* Matrix4f::initOrthographic(const float& left, const float& right, const float& top, const float& bottom, const float& near, const float& far) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		float width = right - left;
		float height = top - bottom;
		float depth = far - near;

		m[0] = 2 / width; m[4] = 0;			 m[8] = 0;			 m[12] = -(right + left) / width;
		m[1] = 0;		  m[5] = 2 / height; m[9] = 0;			 m[13] = -(top + bottom) / height;
		m[2] = 0;		  m[6] = 0;		 	 m[10] = -2 / depth; m[14] = -(far + near) / depth;
		m[3] = 0;		  m[7] = 0;			 m[11] = 0;			 m[15] = 1;

		return mat;
	}

	Matrix4f* Matrix4f::initTranslation(const float& m_x, const float& y, const float& z) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		m[0] = 1; m[4] = 0;	m[8] = 0;   m[12] = m_x;
		m[1] = 0; m[5] = 1;	m[9] = 0;   m[13] = y;
		m[2] = 0; m[6] = 0;	m[10] = 1;  m[14] = z;
		m[3] = 0; m[7] = 0;	m[11] = 0;  m[15] = 1;
		return mat;
	}

	Matrix4f* Matrix4f::initRotation(const float& m_x, const float& y, const float& z) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		Matrix4f rm_x = Matrix4f();
		Matrix4f ry = Matrix4f();
		Matrix4f rz = Matrix4f();

		float m_x_ = Utils::toRadians(m_x);
		float y_ = Utils::toRadians(y);
		float z_ = Utils::toRadians(z);

		rz.m[0] = (float)::cos(z);		rz.m[4] = -(float)::sin(z);			rz.m[8] = 0;					rz.m[12] = 0;
		rz.m[1] = (float)::sin(z);		rz.m[5] = (float)::cos(z);			rz.m[9] = 0;					rz.m[13] = 0;
		rz.m[2] = 0;		     		rz.m[6] = 0;						rz.m[10] = 1;					rz.m[14] = 0;
		rz.m[3] = 0;					rz.m[7] = 0;						rz.m[11] = 0;					rz.m[15] = 1;

		rm_x.m[0] = 1;					rm_x.m[4] = 0;						rm_x.m[8] = 0;					rm_x.m[12] = 0;
		rm_x.m[1] = 0;					rm_x.m[5] = (float)::cos(m_x);		rm_x.m[9] = -(float)::sin(m_x); rm_x.m[13] = 0;
		rm_x.m[2] = 0;					rm_x.m[6] = (float)::sin(m_x);		rm_x.m[10] = (float)::cos(m_x); rm_x.m[14] = 0;
		rm_x.m[3] = 0;					rm_x.m[7] = 0;						rm_x.m[11] = 0;					rm_x.m[15] = 1;

		ry.m[0] = (float)::cos(y);		ry.m[4] = 0;						ry.m[8] = -(float)::sin(y);		ry.m[12] = 0;
		ry.m[1] = 0;					ry.m[5] = 1;						ry.m[9] = 0;					ry.m[13] = 0;
		ry.m[2] = (float)::sin(y);		ry.m[6] = 0;						ry.m[10] = (float)::cos(y);		ry.m[14] = 0;
		ry.m[3] = 0;					ry.m[7] = 0;						ry.m[11] = 0;					ry.m[15] = 1;

		Matrix4f res = rz * (ry * rm_x);
		for (int i = 0; i < 16; i++)
			m[i] = res.m[i];

		return mat;
	}

	Matrix4f* Matrix4f::initRotation(const Vector3f& forward, const Vector3f& up) {
		Vector3f f = forward;
		f.normalize();

		Vector3f r = up;
		r.normalize();
		r = r.cross(f);

		Vector3f u = f.cross(r);

		return Matrix4f::initRotation(f, u, r);
	}

	Matrix4f* Matrix4f::initRotation(const Vector3f& forward, const Vector3f& up, const Vector3f& right) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		Vector3f f = forward;
		Vector3f r = right;
		Vector3f u = up;

		m[0] = r.x;	m[4] = r.y;	m[8] = r.z;	m[12] = 0;
		m[1] = u.x;	m[5] = u.y;	m[9] = u.z;	m[13] = 0;
		m[2] = f.x;	m[6] = f.y;	m[10] = f.z;	m[14] = 0;
		m[3] = 0;	    	m[7] = 0;			m[11] = 0;			m[15] = 1;

		return mat;
	}

	Matrix4f* Matrix4f::initScale(const float& m_x, const float& y, const float& z) {
		Matrix4f* mat = new Matrix4f();
		float* m = mat->m;

		m[0] = m_x; m[4] = 0;	m[8] = 0;   m[12] = 0;
		m[1] = 0; m[5] = y;	m[9] = 0;   m[13] = 0;
		m[2] = 0; m[6] = 0;	m[10] = z;  m[14] = 0;
		m[3] = 0; m[7] = 0;	m[11] = 0;  m[15] = 1;
		return mat;
	}

	float Matrix4f::get(const int& row, const int& column) const {
		return m[(((column - 1) * 4) + row) - 1];
	}

	void Matrix4f::set(const int& row, const int& column, const float& value) {
		m[(((column - 1) * 4) + row) - 1] = value;
	}

	Vector3f Matrix4f::transform(const Vector3f& r) {
		Vector3f res(m[0] * r.x + m[4] * r.y + m[8]  * r.z + m[12],
					 m[1] * r.x + m[5] * r.y + m[9]  * r.z + m[13],
					 m[2] * r.x + m[6] * r.y + m[10] * r.z + m[14]);
		return res;
	}

	Matrix4f Matrix4f::scale(const float& scale) {
		for (int i = 0; i < 16; i++)
			m[i] *= scale;

		return *this;
	}

	Matrix4f Matrix4f::inverse() const{
		Matrix4f inverse = Matrix4f();
		inverse.initIdentity();

		float m[16];

		for (int i = 0; i < 16; i++)
			m[i] = this->m[i];

		m[0]  = m[9]  * m[14] * m[7] - m[13] * m[10] * m[7] + m[13] * m[6] * m[11] - m[5] * m[14] * m[11] - m[9] * m[6] * m[15] + m[5] * m[10] * m[15];
		m[4]  = m[12] * m[10] * m[7] - m[8]  * m[14] * m[7] - m[12] * m[6] * m[11] + m[4] * m[14] * m[11] + m[8] * m[6] * m[15] - m[4] * m[10] * m[15];
		m[8]  = m[8]  * m[13] * m[7] - m[12] * m[9]  * m[7] + m[12] * m[5] * m[11] - m[4] * m[13] * m[11] - m[8] * m[5] * m[15] + m[4] * m[9]  * m[15];
		m[12] = m[12] * m[9]  * m[6] - m[8]  * m[13] * m[6] - m[12] * m[5] * m[10] + m[4] * m[13] * m[10] + m[8] * m[5] * m[14] - m[4] * m[9]  * m[14];
		m[1]  = m[13] * m[10] * m[3] - m[9]  * m[14] * m[3] - m[13] * m[2] * m[11] + m[1] * m[14] * m[11] + m[9] * m[2] * m[15] - m[1] * m[10] * m[15];
		m[5]  = m[8]  * m[14] * m[3] - m[12] * m[10] * m[3] + m[12] * m[2] * m[11] - m[0] * m[14] * m[11] - m[8] * m[2] * m[15] + m[0] * m[10] * m[15];
		m[9]  = m[12] * m[9]  * m[3] - m[8]  * m[13] * m[3] - m[12] * m[1] * m[11] + m[0] * m[13] * m[11] + m[8] * m[1] * m[15] - m[0] * m[9]  * m[15];
		m[13] = m[8]  * m[13] * m[2] - m[12] * m[9]  * m[2] + m[12] * m[1] * m[10] - m[0] * m[13] * m[10] - m[8] * m[1] * m[14] + m[0] * m[9]  * m[14];
		m[2]  = m[5]  * m[14] * m[3] - m[13] * m[6]  * m[3] + m[13] * m[2] * m[7]  - m[1] * m[14] * m[7]  - m[5] * m[2] * m[15] + m[1] * m[6]  * m[15];
		m[6]  = m[12] * m[6]  * m[3] - m[4]  * m[14] * m[3] - m[12] * m[2] * m[7]  + m[0] * m[14] * m[7]  + m[4] * m[2] * m[15] - m[0] * m[6]  * m[15];
		m[10] = m[4]  * m[13] * m[3] - m[12] * m[5]  * m[3] + m[12] * m[1] * m[7]  - m[0] * m[13] * m[7]  - m[4] * m[1] * m[15] + m[0] * m[5]  * m[15];
		m[14] = m[12] * m[5]  * m[2] - m[4]  * m[13] * m[2] - m[12] * m[1] * m[6]  + m[0] * m[13] * m[6]  + m[4] * m[1] * m[14] - m[0] * m[5]  * m[14];
		m[3]  = m[9]  * m[6]  * m[3] - m[5]  * m[10] * m[3] - m[9]  * m[2] * m[7]  + m[1] * m[10] * m[7]  + m[5] * m[2] * m[11] - m[1] * m[6]  * m[11];
		m[7]  = m[4]  * m[10] * m[3] - m[8]  * m[6]  * m[3] + m[8]  * m[2] * m[7]  - m[0] * m[10] * m[7]  - m[4] * m[2] * m[11] + m[0] * m[6]  * m[11];
		m[11] = m[8]  * m[5]  * m[3] - m[4]  * m[9]  * m[3] - m[8]  * m[1] * m[7]  + m[0] * m[9]  * m[7]  + m[4] * m[1] * m[11] - m[0] * m[5]  * m[11];
		m[15] = m[4]  * m[9]  * m[2] - m[8]  * m[5]  * m[2] + m[8]  * m[1] * m[6]  - m[0] * m[9]  * m[6]  - m[4] * m[1] * m[10] + m[0] * m[5]  * m[10];

		for (int i = 0; i < 16; i++)
			inverse.m[i] = m[i];

		inverse.scale(1.0f / (float)this->determinant());

		return inverse;
	}

	double Matrix4f::determinant() const {
		double value = m[12] * m[9] * m[6]  * m[3]  - m[8] * m[13] * m[6]  * m[3]  - m[12] * m[5] * m[10] * m[3]  + m[4] * m[13] * m[10] * m[3]  +
					   m[8]  * m[5] * m[14] * m[3]  - m[4] * m[9]  * m[14] * m[3]  - m[12] * m[9] * m[2]  * m[7]  + m[8] * m[13] * m[2]  * m[7]  +
					   m[12] * m[1] * m[10] * m[7]  - m[0] * m[13] * m[10] * m[7]  - m[8]  * m[1] * m[14] * m[7]  + m[0] * m[9]  * m[14] * m[7]  +
					   m[12] * m[5] * m[2]  * m[11] - m[4] * m[13] * m[2]  * m[11] - m[12] * m[1] * m[6]  * m[11] + m[0] * m[13] * m[6]  * m[11] +
					   m[4]  * m[1] * m[14] * m[11] - m[0] * m[5]  * m[14] * m[11] - m[8]  * m[5] * m[2]  * m[15] + m[4] * m[9]  * m[2]  * m[15] +
					   m[8]  * m[1] * m[6]  * m[15] - m[0] * m[9]  * m[6]  * m[15] - m[4]  * m[1] * m[10] * m[15] + m[0] * m[5]  * m[10] * m[15] ;
		return value;
	}
} }