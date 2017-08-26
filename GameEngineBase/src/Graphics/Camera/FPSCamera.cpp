#include "Pch.h"
#include "Input/InputManager.h"
#include "Application/Application.h"
#include "FPSCamera.h"

namespace gebase { namespace graphics {

	FPSCamera::FPSCamera(const math::Matrix4f& projectionMatrix) : Camera(projectionMatrix), m_MouseSensitivity(0.002f), m_Speed(0.4f), m_SprintSpeed(m_Speed * 4.0f), m_MouseWasGrabbed(false)
	{
		m_Position = math::Vector3f();
		m_Rotation = math::Quaternion(math::Vector3f(90.0f, 0.0f, 0.0f), 1.0f);
	}

	FPSCamera::~FPSCamera()
	{

	}

	void FPSCamera::Focus()
	{
		input::Input::getInputManager()->setMouseCursor(GE_NO_CURSOR);
		input::Input::getInputManager()->setMouseGrabbed(true);
	}

	void FPSCamera::Update(float delta)
	{
		math::Vector2f windowSize = Application::getApplication().getWindowSize();
		math::Vector2f windowCent = math::Vector2f((float)((int32)(windowSize.x / 2.0f)), (float)((int32)(windowSize.y / 2.0f)));

		if (input::Input::isMouseButtonPressed(GE_MOUSE_RIGHT))
		{
			if (!input::Input::getInputManager()->isMouseGrabbed())
			{
				input::Input::getInputManager()->setMouseGrabbed(true);
				input::Input::getInputManager()->setMouseCursor(GE_NO_CURSOR);
			}
		}

		if (input::Input::getInputManager()->isMouseGrabbed())
		{
			

			math::Vector2f mPos = input::Input::getInputManager()->getMousePosition();
			mPos.x -= windowCent.x;
			mPos.y -= windowCent.y;

			if (m_MouseWasGrabbed)
			{
				m_Rotation *= math::Quaternion(math::Vector3f(0.0f, 1.0f, 0.0f), (float)math::Utils::toRadians(mPos.x * m_MouseSensitivity)).normalize();
				m_Rotation *= math::Quaternion(getRightDirection(m_Rotation), (float)math::Utils::toRadians(-mPos.y * m_MouseSensitivity)).normalize();
			}

			m_MouseWasGrabbed = true;
			input::Input::getInputManager()->setMousePosition(windowCent);

			math::Vector3f forward = getForwardDirection(m_Rotation);
			math::Vector3f right = getRightDirection(m_Rotation);
			math::Vector3f up = math::Vector3f(0.0f, 1.0f, 0.0f);

			float speed = input::Input::isKeyPressed(GE_KEY_SHIFT) ? m_SprintSpeed : m_Speed;

			if (input::Input::isKeyPressed(GE_KEY_W))
				Translate(forward, speed * delta);
			else if (input::Input::isKeyPressed(GE_KEY_S))
				Translate(forward, -speed * delta);

			if (input::Input::isKeyPressed(GE_KEY_A))
				Translate(right, -speed * delta);
			else if (input::Input::isKeyPressed(GE_KEY_D))
				Translate(right, speed * delta);

			if (input::Input::isKeyPressed(GE_KEY_SPACE))
				Translate(up, speed * delta);
			else if (input::Input::isKeyPressed(GE_KEY_CONTROL))
				Translate(up, -speed * delta);
		}

		if (input::Input::isKeyPressed(GE_KEY_ESCAPE))
		{
			input::Input::getInputManager()->setMouseGrabbed(false);
			input::Input::getInputManager()->setMouseCursor(1);
			m_MouseWasGrabbed = false;
		}
	}

} }