#include "ge.h"
#include "Button.h"

#include "Debug/DebugMenu.h"
#include "Graphics/Font/FontManager.h"

namespace gebase { namespace graphics { namespace ui {

	Button::Button(const String& label, const math::Rectangle& bounds, const ActionHandler& action) : Widget(bounds), m_Label(label), m_ActionHandler(action), m_State(ButtonState::UNPRESSED)
	{
		m_Font = FontManager::Get(24);
	}

	void Button::OnUpdate(float delta)
	{

	}

	void Button::OnRender(Renderer2D& renderer)
	{
		float hp = debug::DebugMenu::getSettings().horizontalPadding * 0.5f;

		renderer.DrawRect(m_Bounds);
		renderer.FillRect(m_Bounds, m_State == ButtonState::PRESSED ? 0xCFBBBBBB : 0xCF5F5F5F );
		renderer.DrawString(m_Label, m_Bounds.position - math::Vector2f(m_Bounds.width - hp, m_Font->getHeight(m_Label) * 0.5f), *m_Font);
	}

	void Button::OnAction()
	{
		m_ActionHandler();
	}

	bool Button::OnMousePressed(events::MousePressedEvent& e)
	{
		m_State = ButtonState::PRESSED;
		return true;
	}

	bool Button::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		if (m_State == ButtonState::PRESSED)
			OnAction();

		m_State = ButtonState::UNPRESSED;
		return true;
	}

	bool Button::OnMouseMoved(events::MouseMovedEvent& e)
	{
		math::Vector2f mPos(e.getX() * (32.0f / Window::getWindowClass(nullptr)->getWidth()), 18.0f - e.getY() * (18.0f / Window::getWindowClass(nullptr)->getHeight()));
		
		if (m_State == ButtonState::PRESSED && !m_Bounds.Contains(mPos))
			m_State = ButtonState::UNPRESSED;

		return true;
	}

} } }