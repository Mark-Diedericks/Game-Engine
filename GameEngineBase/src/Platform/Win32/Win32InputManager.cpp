#include "ge.h"
#include "Common.h"
#include "Input/InputManager.h"
#include "Application/Window.h"
#include "Application/Application.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include <Windows.h>
#include <Windowsx.h>

namespace gebase {
	using namespace events;

	extern HWND hWnd;

	void InputManager::PlatformUpdate()
	{
		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(hWnd, &mouse);

		if (!(mouse.x == m_MouseX && mouse.y == m_MouseY))
		{
			m_EventCallback(MouseMovedEvent((float)mouse.x, (float)mouse.y, m_MouseButtons[GE_MOUSE_LEFT]));
			m_MouseX = mouse.x;
			m_MouseY = mouse.y;
		}
	}

	void InputManager::setMousePosition(const long mx, const long my)
	{
		POINT p = { mx, my };
		ClientToScreen(hWnd, &p);
		SetCursorPos(p.x, p.y);
	}

	void InputManager::setMouseCursor(int32 cursor)
	{
		if (cursor == GE_NO_CURSOR)
		{
			SetCursor(GE_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
	}

	void KeyCallback(InputManager* inputManager, int32 flags, int32 keyCode, uint message)
	{
		bool prsd = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
		inputManager->m_KeyState[keyCode] = prsd;

		bool repeat = (flags >> 30) & 1;
		int32 mod = 0;

		switch (keyCode)
		{
		case GE_KEY_CONTROL:
			mod = GE_MODIFIER_LEFT_CONTROL;
			break;
		case GE_KEY_ALT:
			mod = GE_MODIFIER_LEFT_ALT;
			break;
		case GE_KEY_SHIFT:
			mod = GE_MODIFIER_LEFT_SHIFT;
			break;
		}

		if (prsd)
			inputManager->m_KeyModifiers |= mod;
		else
			inputManager->m_KeyModifiers &= ~(mod);

		if (prsd)
			inputManager->m_EventCallback(KeyPressedEvent(keyCode, repeat, inputManager->m_KeyModifiers));
		else
			inputManager->m_EventCallback(KeyReleasedEvent(keyCode));
	}

	void MouseButtonCallback(InputManager* inputManager, int32 button, int32 x, int32 y)
	{
		bool down = false;

		switch(button)
		{
		case WM_LBUTTONDOWN:
			SetCapture(hWnd);
			button = GE_MOUSE_LEFT;
			down = true;
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
			button = GE_MOUSE_LEFT;
			down = false;
			break;
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			button = GE_MOUSE_RIGHT;
			down = true;
			break;
		case WM_RBUTTONUP:
			ReleaseCapture();
			button = GE_MOUSE_RIGHT;
			down = false;
			break;
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			button = GE_MOUSE_MIDDLE;
			down = true;
			break;
		case WM_MBUTTONUP:
			ReleaseCapture();
			button = GE_MOUSE_MIDDLE;
			down = false;
			break;
		}

		inputManager->m_MouseButtons[button] = down;
		inputManager->m_MouseX = (float)x;
		inputManager->m_MouseY = (float)y;

		if (down)
			inputManager->m_EventCallback(MousePressedEvent(button, (float)x, (float)y));
		else
			inputManager->m_EventCallback(MouseReleasedEvent(button, (float)x, (float)y));
	}
} 