#include "ge.h"
#include "Widget.h"

namespace gebase { namespace graphics { namespace ui {

	Widget::Widget(const math::Rectangle& bounds) : m_Bounds(bounds), m_Active(true), m_Focused(false)
	{

	}

	void Widget::OnUpdate(float delta)
	{

	}

	void Widget::OnRender(Renderer2D& renderer)
	{

	}

	bool Widget::OnMousePressed(events::MousePressedEvent& e)
	{
		return false;
	}

	bool Widget::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		return false;
	}

	bool Widget::OnMouseMoved(events::MouseMovedEvent& e)
	{
		return false;
	}

} } }