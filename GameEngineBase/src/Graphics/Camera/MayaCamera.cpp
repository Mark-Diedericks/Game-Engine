#include "ge.h"
#include "MayaCamera.h"

#include "Application/Window.h"
#include "Input/InputManager.h"

namespace gebase {
	namespace graphics {

		MayaCamera::MayaCamera(const math::Matrix4f& projectionMatrix) : Camera(projectionMatrix)
		{
			m_PanSpeed = 0.0015f;
			m_RotationSpeed = 0.002f;
			m_ZoomSpeed = 0.2f;

			m_Position = math::Vector3f(0.0f, 25.0f, -25.0f);
			m_Rotation = math::Vector3f(90.0f, 0.0f, 0.0f);

			m_FocalPoint = math::Vector3f();
			m_Distance = (m_Position - m_FocalPoint).getLength();

			m_Pitch = GE_PI / 4.0f;
			m_Yaw = 3.0f * GE_PI / 4.0f;
		}

		void MayaCamera::Focus()
		{
			Input::getInputManager()->setMouseCursor(1);
		}

		void MayaCamera::Update(float delta)
		{
			if (Input::isKeyPressed(GE_KEY_ALT))
			{
				float mDeltaX = Input::getMouseX() - m_InitialMouseX;
				float mDeltaY = Input::getMouseY() - m_InitialMouseY;

				if (Input::isMouseButtonPressed(GE_MOUSE_MIDDLE))
					MousePan(mDeltaX, mDeltaY, delta);
				if (Input::isMouseButtonPressed(GE_MOUSE_LEFT))
					MouseRotate(mDeltaX, mDeltaY, delta);
				if (Input::isMouseButtonPressed(GE_MOUSE_RIGHT))
					MouseZoom(mDeltaY, delta);
			}

			m_Position = calculatePosition();

			math::Quaternion orientation = getOrientation();
			m_Rotation = orientation.ToEulerAngles() * (180.0f / GE_PI);

			math::Matrix4f trans = math::Matrix4f::Translation(0.0f, 0.0f, 1.0f);
			math::Matrix4f rot = math::Matrix4f::Rotate(orientation.Conjugate());
			math::Matrix4f loc = math::Matrix4f::Translation(m_Position * -1.0f);

			m_ViewMatrix = trans * rot * loc;
		}


		void MayaCamera::MousePan(const float deltaX, const float deltaY, float tDelta)
		{
			m_FocalPoint += (getRightDirection(getOrientation()) * -1.0f) * deltaX * (m_PanSpeed * tDelta) * m_Distance;
			m_FocalPoint += getUpDirection(getOrientation()) * deltaY * (m_PanSpeed * tDelta) * m_Distance;
		}

		void MayaCamera::MouseRotate(const float deltaX, const float deltaY, float tDelta)
		{
			float yawSign = getUpDirection(getOrientation()).y < 0.0f ? -1.0f : 1.0f;
			m_Pitch += deltaY * m_RotationSpeed;
			m_Yaw += yawSign * deltaX * m_RotationSpeed;
		}

		void MayaCamera::MouseZoom(float delta, float tDelta)
		{
			m_Distance -= delta * (m_ZoomSpeed * tDelta);

			if (m_Distance < 1.0f)
			{
				m_FocalPoint += getForwardDirection(getOrientation());
				m_Distance = 1.0f;
			}
		}

		math::Vector3f MayaCamera::calculatePosition()
		{
			return m_FocalPoint - getForwardDirection(getOrientation()) * m_Distance;
		}

		math::Quaternion MayaCamera::getOrientation() const
		{
			return math::Quaternion::RotationY(-m_Yaw) * math::Quaternion::RotationX(-m_Pitch);
		}

	}
}