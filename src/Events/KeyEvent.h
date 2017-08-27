#pragma once

#include "Event.h"

namespace gebase { namespace events {

	class GE_API KeyEvent : public Event
	{
	protected:
		int32 m_KeyCode;
		int32 m_Count;
	public:
		KeyEvent(int32 keyCode, Event::EventType type);
		inline int32 getKeyCode() { return m_KeyCode; }
		inline static int32 getStaticType() { return (int32)Event::EventType::KEY_PRESSED | (int32)Event::EventType::KEY_RELEASED; }
	};

#pragma warning(disable: 4800)
	class GE_API KeyPressedEvent : public KeyEvent
	{
	private:
		int32 m_Repeat;
		int32 m_Modifiers;
	public:
		KeyPressedEvent(int32 button, int32 repeat, int32 mods);

		inline int32 getRepeat() const { return m_Repeat; }
		inline int32 getModifiers() const { return m_Modifiers; }
		inline bool isModifier(int32 mod) const { return (bool)(m_Modifiers & mod); }
		inline static EventType getStaticType() { return Event::EventType::KEY_PRESSED; }
	};

#pragma warning(disable: 4800)
	class GE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32 button);
		inline static EventType getStaticType() { return Event::EventType::KEY_RELEASED; }
	};

} }