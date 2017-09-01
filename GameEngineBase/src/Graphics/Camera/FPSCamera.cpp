#include "ge.h"
#include "FPSCamera.h"

#include "Input/InputManager.h"
#include "Application/Application.h"
#include "Debug/DebugMenu.h"

namespace gebase { namespace graphics {

	FPSCamera::FPSCamera(const math::Matrix4f& projectionMatrix) : Camera(projectionMatrix), m_MouseSensitivity(0.002f), m_Speed(0.4f), m_SprintSpeed(m_Speed * 4.0f), m_MouseWasGrabbed(false)
	{
		m_Position = math::Vector3f(0.0f, 25.0f, -25.0f);
		m_Rotation = math::Quaternion(math::Vector3f(90.0f, 0.0f, 0.0f), 1.0f);

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

			float xangle = (windowCent.x - mPos.x);
			float yangle = (windowCent.y - mPos.y);

			mPos.x -= windowCent.x;
			mPos.y -= windowCent.y;

			if (m_MouseWasGrabbed)
			{
				m_Rotation *= math::Quaternion(math::Vector3f(0.0f, 1.0f, 0.0f), (float)math::Utils::toRadians(xangle * m_MouseSensitivity * delta)).normalize();
				m_Rotation *= math::Quaternion(getOrientation().getRotateRight(), (float)math::Utils::toRadians(-yangle * m_MouseSensitivity * delta)).normalize();
			}

			m_MouseWasGrabbed = true;
			Input::getInputManager()->setMousePosition(windowCent);

			math::Quaternion orientation = getOrientation();

			math::Vector3f forward = getForwardDirection(orientation);
			math::Vector3f right = getRightDirection(orientation);
			math::Vector3f up = math::Vector3f(0.0f, 1.0f, 0.0f);

			float speed = Input::isKeyPressed(GE_KEY_SHIFT) ? m_SprintSpeed : m_Speed;

			if (Input::isKeyPressed(GE_KEY_W))
				Translate(forward.normalize(), speed * delta);
			else if (Input::isKeyPressed(GE_KEY_S))
				Translate(forward.normalize(), -speed * delta);

			if (Input::isKeyPressed(GE_KEY_A))
				Translate(right.normalize(), -speed * delta);
			else if (Input::isKeyPressed(GE_KEY_D))
				Translate(right.normalize(), speed * delta);

			if (Input::isKeyPressed(GE_KEY_SPACE))
				Translate(up.normalize(), speed * delta);
			else if (Input::isKeyPressed(GE_KEY_CONTROL))
				Translate(up.normalize(), -speed * delta);

			math::Matrix4f rot = orientation.conjugate().toRotationMatrix();
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
		return m_Rotation;
	}

} }