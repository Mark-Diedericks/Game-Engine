#include "ge.h"
#include "KeyEvent.h"

namespace gebase { namespace events {

	KeyEvent::KeyEvent(int32 keyCode, Event::EventType type) : Event(type), m_KeyCode(keyCode) {}
	KeyPressedEvent::KeyPressedEvent(int32 button, int32 repeat, int32 mods) : KeyEvent(button, KeyPressedEvent::getStaticType()), m_Repeat(repeat), m_Modifiers(mods) {}
	KeyReleasedEvent::KeyReleasedEvent(int32 button) : KeyEvent(button, KeyReleasedEvent::getStaticType()) {}

} }