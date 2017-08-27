#include "ge.h"
#include "Slider.h"

namespace gebase { namespace graphics { namespace ui {

	using namespace math;

	Slider::Slider(const math::Rectangle& bounds, bool vertical) : Widget(bounds), m_Vertical(vertical), m_Value(0.0f), m_HeadOffset(0.0f), m_State(SliderState::UNPRESSED), m_Callback(&Slider::NoCallback)
	{
		float size = vertical ? bounds.width : bounds.height;
		m_HeadBounds = Rectangle(bounds.x, bounds.y, size, size);
	}

	Slider::Slider(const math::Rectangle& bounds, float value, const ValueChangedCallback& callback, bool vertical) : Widget(bounds), m_Vertical(vertical), m_Value(value), m_Callback(callback), m_State(SliderState::UNPRESSED), m_HeadOffset(0.0f)
	{
		float size = vertical ? bounds.width : bounds.height;
		m_HeadBounds = Rectangle(bounds.x, bounds.y, size, size);
	}

	void Slider::setValue(float value)
	{
		value = Utils::clamp(value, 0.0f, 1.0f);
		m_Value = value;
		m_Callback(value);
	}

	void Slider::OnUpdate(float delta)
	{
		if (!input::Input::isMouseButtonPressed(GE_MOUSE_LEFT))
			m_State = SliderState::UNPRESSED;
	
		m_Vertical ? m_HeadBounds.y = m_Bounds.getMinimumBound().y + m_HeadBounds.height + m_Value * (m_Bounds.height * 2.0f - m_HeadBounds.height * 2.0f) : m_HeadBounds.x = m_Bounds.getMinimumBound().x + m_HeadBounds.width + m_Value * (m_Bounds.width * 2.0f - m_HeadBounds.width * 2.0f);
	}

	void Slider::OnRender(Renderer2D& renderer)
	{
		renderer.FillRect(m_Bounds, 0xCF7F7F7F);
		renderer.DrawRect(m_Bounds);
		renderer.FillRect(m_HeadBounds, 0xCFBFBFBF);
		renderer.DrawRect(m_HeadBounds);

		Vector2f offset = m_Vertical ? Vector2f(0, m_Bounds.size.y) : Vector2f(m_Bounds.size.x, 0);
		renderer.DrawLine(m_Bounds.position - offset, m_Bounds.position + offset);
	}

	bool Slider::OnMousePressed(events::MousePressedEvent& e)
	{
		Vector2f mPos(e.getX() * (32.0f / Window::getWindowClass(nullptr)->getWidth()), 18.0f - e.getY() * (18.0f / Window::getWindowClass(nullptr)->getHeight()));
		m_State = SliderState::PRESSED;

		if (m_HeadBounds.Contains(mPos))
		{
			m_HeadOffset = m_Vertical ? (mPos.y - m_HeadBounds.y) : (mPos.x - m_HeadBounds.x);
			m_State = SliderState::PRESSEDHEAD;
		}

		return true;
	}

	bool Slider::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		m_State = SliderState::UNPRESSED;
		return true;
	}

	bool Slider::OnMouseMoved(events::MouseMovedEvent& e)
	{
		Vector2f mPos(e.getX() * (32.0f / Window::getWindowClass(nullptr)->getWidth()), 18.0f - e.getY() * (18.0f / Window::getWindowClass(nullptr)->getHeight()));

		if (m_State == SliderState::PRESSEDHEAD)
			m_Vertical ? setValue((mPos.y - m_Bounds.getMinimumBound().y - m_HeadOffset) / m_Bounds.getMaximumBound().y) : setValue((mPos.x - m_Bounds.getMinimumBound().x - m_HeadOffset) / m_Bounds.getMaximumBound().x);

		return true;
	}

} } }