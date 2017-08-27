#include "ge.h"
#include "MouseEvent.h"

namespace gebase { namespace events {

	MouseButtonEvent::MouseButtonEvent(int32 button, float x, float y, Event::EventType type) : Event(type), m_Button(button), m_Position(math::Vector2f(x, y)) {}
	MouseMovedEvent::MouseMovedEvent(float x, float y, bool dragged) : Event(MouseMovedEvent::getStaticType()), m_Dragged(dragged), m_Position(math::Vector2f(x, y)) {}

	MousePressedEvent::MousePressedEvent(int32 button, float x, float y) : MouseButtonEvent(button, x, y, MousePressedEvent::getStaticType()) {}
	MouseReleasedEvent::MouseReleasedEvent(int32 button, float x, float y) : MouseButtonEvent(button, x, y, MouseReleasedEvent::getStaticType()) {}
} }