#pragma once

#include "Math/Vector2f.h"
#include "Event.h"

namespace gebase { namespace events {

	class GE_API WindowResizeEvent : public Event
	{
	protected:
		math::Vector2f m_Size;
	public:
		WindowResizeEvent(uint width, uint height);

		inline const math::Vector2f& getSize() { return m_Size; }
		inline const uint getWidth() { return (uint)m_Size.x; }
		inline const uint getHeight() { return (uint)m_Size.y; }
		inline static EventType getStaticType() { return Event::EventType::WINDOW_RESIZE; }
	};

} }