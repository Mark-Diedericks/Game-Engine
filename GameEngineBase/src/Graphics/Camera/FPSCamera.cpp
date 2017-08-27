#include "ge.h"
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
		Input::getInputManager()->setMouseCursor(GE_NO_CURSOR);
		Input::getInputManager()->setMouseGrabbed(true);
	}

	void FPSCamera::Update(float delta)
	{
		math::Vector2f windowSize = Application::getApplication().getWindowSize();
		math::Vector2f windowCent = math::Vector2f((float)((int32)(windowSize.x / 2.0f)), (float)((int32)(windowSize.y / 2.0f)));

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
			

			math::Vector2f mPos = Input::getInputManager()->getMousePosition();
			mPos.x -= windowCent.x;
			mPos.y -= windowCent.y;

			if (m_MouseWasGrabbed)
			{
				m_Rotation *= math::Quaternion(math::Vector3f(0.0f, 1.0f, 0.0f), (float)math::Utils::toRadians(mPos.x * m_MouseSensitivity)).normalize();
				m_Rotation *= math::Quaternion(getRightDirection(m_Rotation), (float)math::Utils::toRadians(-mPos.y * m_MouseSensitivity)).normalize();
			}

			m_MouseWasGrabbed = true;
			Input::getInputManager()->setMousePosition(windowCent);

			math::Vector3f forward = getForwardDirection(m_Rotation);
			math::Vector3f right = getRightDirection(m_Rotation);
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
		}

		if (Input::isKeyPressed(GE_KEY_ESCAPE))
		{
			Input::getInputManager()->setMouseGrabbed(false);
			Input::getInputManager()->setMouseCursor(1);
			m_MouseWasGrabbed = false;
		}
	}

} }