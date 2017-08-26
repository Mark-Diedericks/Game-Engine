#pragma once

#include "Pch.h"
#include "Common.h"
#include "String.h"

namespace gebase { namespace events {

#undef MOUSE_PRESSED
#undef MOUSE_RELEASED
#undef MOUSE_MOVED

#undef KEY_PRESSED
#undef KEY_RELEASED

	class GE_API Event {
	private:
		friend class EventDispatcher;
	public:
		enum class EventType {
			MOUSE_PRESSED = BIT(2),
			MOUSE_RELEASED = BIT(3),
			MOUSE_MOVED = BIT(4),

			KEY_PRESSED = BIT(0),
			KEY_RELEASED = BIT(1),

			WINDOW_RESIZE = BIT(5)
		};

	protected:
		bool m_Handled;
		EventType m_Type;
		Event(EventType type);
	public:
		inline EventType getEventType() const { return m_Type; }
		inline bool isHandled() const { return m_Handled; }

		virtual String ToString() const;
		static String TypeToString(EventType type);
	};

} }