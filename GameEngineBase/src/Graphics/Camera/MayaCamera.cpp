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

			math::Quaternion orientation = getOrientation();
			m_Rotation = orientation.ToEulerAngles() * (180.0f / GE_PI);

			math::Matrix4f trans = math::Matrix4f::Translation(0.0f, 0.0f, 1.0f);
			math::Matrix4f rot = orientation.Conjugate().toRotationMatrix();
			math::Matrix4f loc = math::Matrix4f::Translation(m_Position * -1.0f);

			m_ViewMatrix = trans * rot * loc;
		}


		void MayaCamera::MousePan(const math::Vector2f& delta, float tDelta)
		{
			m_FocalPoint += (getRightDirection(getOrientation()) * -1.0f) * delta.x * (m_PanSpeed * tDelta) * m_Distance;
			m_FocalPoint += getUpDirection(getOrientation()) * delta.y * (m_PanSpeed * tDelta) * m_Distance;
		}

		void MayaCamera::MouseRotate(const math::Vector2f& delta, float tDelta)
		{
			float yawSign = getUpDirection(getOrientation()).y < 0.0f ? -1.0f : 1.0f;
			m_Pitch += delta.y * m_RotationSpeed;
			m_Yaw += yawSign * delta.x * m_RotationSpeed;
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