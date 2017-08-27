#include "ge.h"
#include "InputManager.h"

namespace gebase { namespace input {
	InputManager* Input::s_InputManager = nullptr;

	InputManager::InputManager()
	{
		clearKeys();
		clearMouseButtons();

		m_MouseGrabbed = true;
		Input::s_InputManager = this;
	}

	void InputManager::Update()
	{
		for (int32 i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

		memcpy(m_LastKeyState, m_KeyState, MAX_KEYS);
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	void InputManager::clearKeys()
	{
		for (int32 i = 0; i < MAX_KEYS; i++) 
		{
			m_KeyState[i] = false;
			m_LastKeyState[i] = false;
		}

		m_KeyModifiers = 0;
	}

	void InputManager::clearMouseButtons()
	{
		for (int32 i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseClicked[i] = false;
			m_MouseState[i] = false;
		}
	}

	bool InputManager::isKeyPressed(uint keyCode) const
	{
		if (keyCode >= MAX_KEYS)
			return false;

		return m_KeyState[keyCode];
	}

	bool InputManager::isMouseButtonPressed(uint button) const
	{
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseButtons[button];
	}

	bool InputManager::isMouseButtonClicked(uint button) const
	{
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseClicked[button];
	}

	const math::Vector2f& InputManager::getMousePosition() const
	{
		return m_MousePosition;
	}

	const bool InputManager::isMouseGrabbed() const
	{
		return m_MouseGrabbed;
	}

	void InputManager::setMouseGrabbed(bool grabbed)
	{
		m_MouseGrabbed = grabbed;
	}
} }