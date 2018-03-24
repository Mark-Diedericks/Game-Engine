#include "ge.h"
#include "FPSCamera.h"

#include "Input/InputManager.h"
#include "Application/Application.h"
#include "Debug/DebugMenu.h"

namespace gebase {
	namespace graphics {

		FPSCamera::FPSCamera(const math::Matrix4f& projectionMatrix) : Camera(projectionMatrix), m_MouseSensitivity(0.0005f), m_Speed(0.4f), m_SprintSpeed(m_Speed * 4.0f), m_MouseWasGrabbed(false)
		{
			m_Position = math::Vector3f(0.0f, 25.0f, -25.0f);
			m_Rotation = math::Vector3f(90.0f, 0.0f, 0.0f);

			m_Pitch = 0.7f;
			m_Yaw = 2.4f;

			debug::DebugMenu::Add("Camera/FPS Camera Speed", &m_Speed);
			debug::DebugMenu::Add("Camera/FPS Camera Sprint Speed", &m_SprintSpeed);
		}

		FPSCamera::~FPSCamera()
		{
			debug::DebugMenu::Remove("Camera/FPS Camera Speed");
			debug::DebugMenu::Remove("Camera/FPS Camera Sprint Speed");
		}

		void FPSCamera::Focus()
		{
			Input::getInputManager()->setMouseCursor(GE_NO_CURSOR);
		}

		void FPSCamera::Update(float delta)
		{
			float wcx = (float)Application::getApplication().getWindowCenterX();
			float wcy = (float)Application::getApplication().getWindowCenterY();

			if (Input::isMouseButtonPressed(GE_MOUSE_RIGHT))
			{
				if (!Input::getInputManager()->isMouseGrabbed())
				{
					Input::getInputManager()->setMouseGrabbed(true);
					Input::getInputManager()->setMouseCursor(GE_NO_CURSOR);
				}
			}

			if (Input::getInputManager()->isMouseGrabbed())
			{
				float mx = Input::getInputManager()->getMouseX() - wcx;
				float my = Input::getInputManager()->getMouseY() - wcy;

				if (m_MouseWasGrabbed)
				{
					m_Pitch += my * m_MouseSensitivity * delta;
					m_Yaw += mx * m_MouseSensitivity * delta;
				}

				m_MouseWasGrabbed = true;
				Input::getInputManager()->setMousePosition((long)wcx, (long)wcy);

				math::Quaternion orientation = getOrientation();
				m_Rotation = orientation.ToEulerAngles() * (180.0f / GE_PI);

				math::Vector3f forward = getForwardDirection(orientation);
				math::Vector3f right = getRightDirection(orientation);
				math::Vector3f up = math::Vector3f(0.0f, 1.0f, 0.0f);

				float speed = Input::isKeyPressed(GE_KEY_SHIFT) ? m_SprintSpeed : m_Speed;

				if (Input::isKeyPressed(GE_KEY_W))
					Translate(forward, speed * delta);
				else if (Input::isKeyPressed(GE_KEY_S))
					Translate(forward, -speed * delta);

				if (Input::isKeyPressed(GE_KEY_A))
					Translate(right, -speed * delta);
				else if (Input::isKeyPressed(GE_KEY_D))
					Translate(right, speed * delta);

				if (Input::isKeyPressed(GE_KEY_SPACE))
					Translate(up, speed * delta);
				else if (Input::isKeyPressed(GE_KEY_CONTROL))
					Translate(up, -speed * delta);

				math::Matrix4f rot = math::Matrix4f::Rotate(orientation.Conjugate());
				math::Matrix4f loc = math::Matrix4f::Translation(m_Position * -1.0f);
				m_ViewMatrix = rot * loc;
			}

			if (Input::isKeyPressed(GE_KEY_ESCAPE))
			{
				Input::getInputManager()->setMouseGrabbed(false);
				Input::getInputManager()->setMouseCursor(1);
				m_MouseWasGrabbed = false;
			}
		}

		math::Quaternion FPSCamera::getOrientation() const
		{
			return math::Quaternion::RotationY(-m_Yaw) * math::Quaternion::RotationX(-m_Pitch);
		}

	}
}