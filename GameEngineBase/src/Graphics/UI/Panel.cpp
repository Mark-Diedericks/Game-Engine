#include "ge.h"
#include "Panel.h"
#include "Widget.h"
#include "Application/Application.h"
#include "Math/Maths.h"
#include "Graphics/Shader/ShaderFactory.h"

namespace gebase { namespace graphics { namespace ui {

	using namespace math;
	using namespace events;

	Panel::Panel() : Layer2D(Matrix4f::Orthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f))
	{
		Application::getApplication().PushOverlay(this);
	}

	Panel::~Panel()
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
			delete m_Widgets[i];

		Application::getApplication().PopOverlay(this);
	}

	Widget* Panel::Add(Widget* widget)
	{
		m_Widgets.push_back(widget);
		return widget;
	}

	void Panel::Remove(Widget* widget)
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			if (m_Widgets[i] == widget)
			{
				delete m_Widgets[i];
				m_Widgets.erase(m_Widgets.begin() + i);
				break;
			}
		}
	}

	void Panel::Clear()
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
			gedel m_Widgets[i];

		m_Widgets.clear();
	}

	void Panel::OnUpdate(float delta)
	{
		for (Widget* widget : m_Widgets)
		{
			if (widget->isActive())
				widget->OnUpdate(delta);
		}
	}

	void Panel::OnRender(Renderer2D& renderer)
	{
		for (Widget* widget : m_Widgets)
		{
			if (widget->isActive())
				widget->OnRender(renderer);
		}
	}

	void Panel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MousePressedEvent>(METHOD(&Panel::OnMousePressedEvent));
		dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&Panel::OnMouseReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(METHOD(&Panel::OnMouseMovedEvent));
		
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return Layer2D::OnResize(e.getWidth(), e.getHeight()); });
	}

	bool Panel::OnMousePressedEvent(MousePressedEvent& e)
	{
		Vector2f mPos(e.getX() * (32.0f / Window::getWindowClass(nullptr)->getWidth()), 18.0f - e.getY() * (18.0f / Window::getWindowClass(nullptr)->getHeight()));

		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->getBounds().Contains(mPos))
			{
				if (widget->OnMousePressed(e))
					return true;
			}
		}

		return false;
	}

	bool Panel::OnMouseReleasedEvent(MouseReleasedEvent& e)
	{
		Vector2f mPos(e.getX() * (32.0f / Window::getWindowClass(nullptr)->getWidth()), 18.0f - e.getY() * (18.0f / Window::getWindowClass(nullptr)->getHeight()));

		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->getBounds().Contains(mPos))
			{
				if (widget->OnMouseReleased(e))
					return true;
			}
		}

		return false;
	}

	bool Panel::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->OnMouseMoved(e))
				return true;
		}

		return false;
	}

} } }