#pragma once

#include "Common.h"
#include "Panel.h"
#include "Math/Maths.h"

namespace gebase { namespace graphics { namespace ui {

	class GE_API Widget
	{
	private:
		Widget() {}
	protected:
		bool m_Active;
		bool m_Focused;

		Panel* m_Panel;
		math::Rectangle m_Bounds;

		Widget(const math::Rectangle& bounds);
	public:
		virtual bool OnMousePressed(events::MousePressedEvent& e);
		virtual bool OnMouseReleased(events::MouseReleasedEvent& e);
		virtual bool OnMouseMoved(events::MouseMovedEvent& e);

		virtual void OnUpdate(float delta);
		virtual void OnRender(Renderer2D& renderer);

		inline bool isActive() const { return m_Active; }
		inline void setActive(bool active) { m_Active = active; }

		inline const math::Rectangle& getBounds() const { return m_Bounds; }
		inline math::Rectangle& getBounds() { return m_Bounds; }
		inline void setBounds(const math::Rectangle& bounds) { m_Bounds = bounds; }
	};

} } }