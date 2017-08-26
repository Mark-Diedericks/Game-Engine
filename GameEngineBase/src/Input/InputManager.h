#pragma once

#include "Pch.h"
#include "Common.h"
#include "Math/Maths.h"
#include "Events/Event.h"

namespace gebase { namespace input {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32
	
	typedef std::function<void(events::Event& event)> WindowEventCallback;

	class Window;

	class GE_API InputManager
	{
	private:
		friend class Window;
		friend void KeyCallback(InputManager* inputManager, int32 flags, int32 key, uint message);
		friend void MouseButtonCallback(InputManager* inputManager, int32 button, int32 x, int32 y);

		bool m_KeyState[MAX_KEYS];
		bool m_LastKeyState[MAX_KEYS];
		
		bool m_MouseButtons[MAX_BUTTONS];
		bool m_MouseState[MAX_BUTTONS];
		bool m_MouseClicked[MAX_BUTTONS];

		bool m_MouseGrabbed;
		int32 m_KeyModifiers;

		math::Vector2f m_MousePosition;
		WindowEventCallback m_EventCallback;
	public:
		InputManager();
		inline void setEventCallback(const WindowEventCallback& eventCallback) { m_EventCallback = eventCallback; }

		void Update();
		void PlatformUpdate();

		bool isKeyPressed(uint keyCode) const;
		bool isMouseButtonPressed(uint button) const;
		bool isMouseButtonClicked(uint button) const;

		const math::Vector2f& getMousePosition() const;
		void setMousePosition(const math::Vector2f& position);

		const bool isMouseGrabbed() const;
		void setMouseGrabbed(bool grabbed);
		void setMouseCursor(int32 cursor);

		void clearKeys();
		void clearMouseButtons();
	};

	class GE_API Input
	{
	private:
		friend class InputManager;
		static InputManager* s_InputManager;
	public:
		inline static bool isKeyPressed(uint keyCode) { return s_InputManager->isKeyPressed(keyCode); }
		inline static bool isMouseButtonPressed(uint button) { return s_InputManager->isMouseButtonPressed(button); }
		inline static bool isMouseButtonClicked(uint button) { return s_InputManager->isMouseButtonClicked(button); }

		inline static const math::Vector2f getMousePosition() { return s_InputManager->getMousePosition(); }
		inline static InputManager* getInputManager() { return s_InputManager; }
	};
} }

//KEY DEFINES - TheChernoProject
#define GE_MOUSE_LEFT	  0x00
#define GE_MOUSE_MIDDLE	  0x01
#define GE_MOUSE_RIGHT    0x02

#define GE_NO_CURSOR	  NULL

#define GE_MODIFIER_LEFT_CONTROL	BIT(0)
#define GE_MODIFIER_LEFT_ALT		BIT(1)
#define GE_MODIFIER_LEFT_SHIFT		BIT(2)
#define GE_MODIFIER_RIGHT_CONTROL	BIT(3)
#define GE_MODIFIER_RIGHT_ALT		BIT(4)
#define GE_MODIFIER_RIGHT_SHIFT		BIT(5)

#define GE_KEY_TAB			  0x09

#define GE_KEY_0			  0x30
#define GE_KEY_1			  0x31
#define GE_KEY_2			  0x32
#define GE_KEY_3			  0x33
#define GE_KEY_4			  0x34
#define GE_KEY_5			  0x35
#define GE_KEY_6			  0x36
#define GE_KEY_7			  0x37
#define GE_KEY_8			  0x38
#define GE_KEY_9			  0x39

#define GE_KEY_A			  0x41
#define GE_KEY_B			  0x42
#define GE_KEY_C			  0x43
#define GE_KEY_D			  0x44
#define GE_KEY_E			  0x45
#define GE_KEY_F			  0x46
#define GE_KEY_G			  0x47
#define GE_KEY_H			  0x48
#define GE_KEY_I			  0x49
#define GE_KEY_J			  0x4A
#define GE_KEY_K			  0x4B
#define GE_KEY_L			  0x4C
#define GE_KEY_M			  0x4D
#define GE_KEY_N			  0x4E
#define GE_KEY_O			  0x4F
#define GE_KEY_P			  0x50
#define GE_KEY_Q			  0x51
#define GE_KEY_R			  0x52
#define GE_KEY_S			  0x53
#define GE_KEY_T			  0x54
#define GE_KEY_U			  0x55
#define GE_KEY_V			  0x56
#define GE_KEY_W			  0x57
#define GE_KEY_X			  0x58
#define GE_KEY_Y			  0x59
#define GE_KEY_Z			  0x5A

#define GE_KEY_LBUTTON        0x01
#define GE_KEY_RBUTTON        0x02
#define GE_KEY_CANCEL         0x03
#define GE_KEY_MBUTTON        0x04

#define GE_KEY_ESCAPE         0x1B
#define GE_KEY_SHIFT          0x10
#define GE_KEY_CONTROL        0x11
#define GE_KEY_MENU           0x12
#define GE_KEY_ALT	          GE_KEY_MENU
#define GE_KEY_PAUSE          0x13
#define GE_KEY_CAPITAL        0x14

#define GE_KEY_SPACE          0x20
#define GE_KEY_PRIOR          0x21
#define GE_KEY_NEXT           0x22
#define GE_KEY_END            0x23
#define GE_KEY_HOME           0x24
#define GE_KEY_LEFT           0x25
#define GE_KEY_UP             0x26
#define GE_KEY_RIGHT          0x27
#define GE_KEY_DOWN           0x28
#define GE_KEY_SELECT         0x29
#define GE_KEY_PRINT          0x2A
#define GE_KEY_EXECUTE        0x2B
#define GE_KEY_SNAPSHOT       0x2C
#define GE_KEY_INSERT         0x2D
#define GE_KEY_DELETE         0x2E
#define GE_KEY_HELP           0x2F

#define GE_KEY_NUMPAD0        0x60
#define GE_KEY_NUMPAD1        0x61
#define GE_KEY_NUMPAD2        0x62
#define GE_KEY_NUMPAD3        0x63
#define GE_KEY_NUMPAD4        0x64
#define GE_KEY_NUMPAD5        0x65
#define GE_KEY_NUMPAD6        0x66
#define GE_KEY_NUMPAD7        0x67
#define GE_KEY_NUMPAD8        0x68
#define GE_KEY_NUMPAD9        0x69
#define GE_KEY_MULTIPLY       0x6A
#define GE_KEY_ADD            0x6B
#define GE_KEY_SEPARATOR      0x6C
#define GE_KEY_SUBTRACT       0x6D
#define GE_KEY_DECIMAL        0x6E
#define GE_KEY_DIVIDE         0x6F
#define GE_KEY_F1             0x70
#define GE_KEY_F2             0x71
#define GE_KEY_F3             0x72
#define GE_KEY_F4             0x73
#define GE_KEY_F5             0x74
#define GE_KEY_F6             0x75
#define GE_KEY_F7             0x76
#define GE_KEY_F8             0x77
#define GE_KEY_F9             0x78
#define GE_KEY_F10            0x79
#define GE_KEY_F11            0x7A
#define GE_KEY_F12            0x7B
#define GE_KEY_F13            0x7C
#define GE_KEY_F14            0x7D
#define GE_KEY_F15            0x7E
#define GE_KEY_F16            0x7F
#define GE_KEY_F17            0x80
#define GE_KEY_F18            0x81
#define GE_KEY_F19            0x82
#define GE_KEY_F20            0x83
#define GE_KEY_F21            0x84
#define GE_KEY_F22            0x85
#define GE_KEY_F23            0x86
#define GE_KEY_F24            0x87

#define GE_KEY_NUMLOCK        0x90
#define GE_KEY_SCROLL         0x91

#define GE_KEY_LSHIFT         0xA0
#define GE_KEY_RSHIFT         0xA1
#define GE_KEY_LCONTROL       0xA2
#define GE_KEY_RCONTROL       0xA3
#define GE_KEY_LMENU          0xA4
#define GE_KEY_RMENU          0xA5