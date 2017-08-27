#include "ge.h"
#include "MayaCamera.h"

#include "Application/Window.h"
#include "Input/InputManager.h"

namespace gebase { namespace graphics {

	MayaCamera::MayaCamera(const math::Matrix4f& projectionMatrix) : Camera(projectionMatrix)
	{
		m_PanSpeed = 0.0015f;
		m_RotationSpeed = 0.002f;
		m_ZoomSpeed = 0.2f;

		m_Position = math::Vector3f(-10.0f, 10.0f, -10.0f);
		m_Rotation = math::Quaternion(math::Vector3f(90.0f, 0.0f, 0.0f), 1.0f);

		m_FocalPoint = math::Vector3f();
		m_Distance = (m_Position - m_FocalPoint).getLength();
	}

	void MayaCamera::Focus()
	{
		Input::getInputManager()->setMouseCursor(1);
	}

	void MayaCamera::Update(float delta)
	{
		if (Input::isKeyPressed(GE_KEY_ALT))
		{
			const math::Vector2f& mouse = Input::getMousePosition();
			math::Vector2f& mDelta = mouse - m_InitialMousePosition;

			if (Input::isMouseButtonPressed(GE_MOUSE_MIDDLE))
				MousePan(mDelta, delta);
			if (Input::isMouseButtonPressed(GE_MOUSE_LEFT))
				MouseRotate(mDelta, delta);
			if (Input::isMouseButtonPressed(GE_MOUSE_RIGHT))
				MouseZoom(mDelta.y, delta);
		}

		m_Position = calculatePosition();
		m_ViewMatrix = (*math::Matrix4f::initTranslation(math::Vector3f(0.0f, 0.0f, 1.0f))) * (*m_Rotation.conjugate().toRotationMatrix()) * (*math::Matrix4f::initTranslation(m_Position * -1));
	}


	void MayaCamera::MousePan(const math::Vector2f& delta, float tDelta)
	{
		m_FocalPoint += (getRightDirection(m_Rotation) * -1) * delta.x * (m_PanSpeed * tDelta) * m_Distance;
		m_FocalPoint += getUpDirection(m_Rotation) * delta.y * (m_PanSpeed * tDelta) * m_Distance;
	}

	void MayaCamera::MouseRotate(const math::Vector2f& delta, float tDelta)
	{
		float yawSign = getUpDirection(m_Rotation).y < 0.0f ? -1.0f : 1.0f;
		m_Rotation *= math::Quaternion(math::Vector3f(0.0f, 1.0f, 0.0f), (float)math::Utils::toRadians(delta.x * m_RotationSpeed)).normalize();
		m_Rotation *= math::Quaternion(getRightDirection(m_Rotation), (float)math::Utils::toRadians(-delta.y * m_RotationSpeed)).normalize();
	}

	void MayaCamera::MouseZoom(float delta, float tDelta)
	{
		m_Distance -= delta * (m_ZoomSpeed * tDelta);
		
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += getForwardDirection(m_Rotation);
			m_Distance = 1.0f;
		}
	}

} }