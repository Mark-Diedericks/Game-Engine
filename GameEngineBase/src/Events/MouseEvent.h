#pragma once

#include "Event.h"
#include "Math/Vector2f.h"

namespace gebase {
	namespace events {

		class GE_API MouseMovedEvent : public Event
		{
		private:
			math::Vector2f m_Position;
			bool m_Dragged;
		public:
			MouseMovedEvent(float x, float y, bool dragged);
			
			inline const float getX() { return m_Position.x; }
			inline const float getY() { return m_Position.y; }
			inline const math::Vector2f& getPosition() { return m_Position; }

			inline const bool isDragged() { return m_Dragged; }
			inline static EventType getStaticType() { return Event::EventType::MOUSE_MOVED; }
		};

		class GE_API MouseButtonEvent : public Event
		{
		protected:
			int32 m_Button;
			math::Vector2f m_Position;
			MouseButtonEvent(int32 button, float x, float y, Event::EventType type);
		public:
			inline const float getX() { return m_Position.x; }
			inline const float getY() { return m_Position.y; }
			inline const math::Vector2f& getPosition() { return m_Position; }

			inline const int32 getButton() { return m_Button; }
			inline static int32 getStaticType() { return (int32)Event::EventType::MOUSE_PRESSED | (int32)Event::EventType::MOUSE_RELEASED; }
		};

		class GE_API MousePressedEvent : public MouseButtonEvent
		{
		public:
			MousePressedEvent(int32 button, float x, float y);
			inline static EventType getStaticType() { return Event::EventType::MOUSE_PRESSED; }
		};

		class GE_API MouseReleasedEvent : public MouseButtonEvent
		{
		public:
			MouseReleasedEvent(int32 button, float x, float y);
			inline static EventType getStaticType() { return Event::EventType::MOUSE_RELEASED; }
		};
	}
}