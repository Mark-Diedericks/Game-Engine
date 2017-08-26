#include "Pch.h"
#include "Event.h"

namespace gebase { namespace events {

	Event::Event(EventType type) : m_Type(type), m_Handled(false) {}

	String Event::ToString() const
	{
		return "Event: " + TypeToString(m_Type);
	}

	String Event::TypeToString(EventType type)
	{
		switch (type)
		{
		case gebase::events::Event::EventType::MOUSE_PRESSED:
			return "MOUSE_PRESSED";
		case gebase::events::Event::EventType::MOUSE_RELEASED:
			return "MOUSE_RELEASED";
		case gebase::events::Event::EventType::MOUSE_MOVED:
			return "MOUSE_MOVED";
		case gebase::events::Event::EventType::KEY_PRESSED:
			return "KEY_PRESSED";
		case gebase::events::Event::EventType::KEY_RELEASED:
			return "KEY_RELEASED";
		case gebase::events::Event::EventType::WINDOW_RESIZE:
			return "WINDOW_RESIZE";
		default:
			return "INVALID_EVENT";
		}
	}

} }