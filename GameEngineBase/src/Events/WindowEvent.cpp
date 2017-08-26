#include "Pch.h"
#include "WindowEvent.h"

namespace gebase { namespace events {

	WindowResizeEvent::WindowResizeEvent(uint width, uint height) : Event(WindowResizeEvent::getStaticType()), m_Size(math::Vector2f((float)width, (float)height)) {}

} }