#include "ge.h"
#include "Camera.h"

namespace gebase { namespace graphics {

	Camera::Camera(const math::Matrix4f& projectionMatrix) : m_ProjectionMatrix(projectionMatrix)
	{
		m_ViewMatrix = math::Matrix4f::initIdentity();
		m_Position = math::Vector3f();
		m_Rotation = math::Vector3f();
	}

} }