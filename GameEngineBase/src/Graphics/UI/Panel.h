#pragma once

#include "Common.h"
#include "Events/EventHeader.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/Layer/Layer2D.h"

namespace gebase { namespace graphics { namespace ui {

	class Widget;

	class Panel : public Layer2D
	{
	private:
		std::vector<Widget*> m_Widgets;
	public:
		Panel();
		~Panel();

		Widget* Add(Widget* widget);
		void Remove(Widget* widget);
		void Clear();

		bool OnMousePressedEvent(events::MousePressedEvent& e);
		bool OnMouseReleasedEvent(events::MouseReleasedEvent& e);
		bool OnMouseMovedEvent(events::MouseMovedEvent& e);

		void OnEvent(events::Event& e) override;
		void OnUpdate(float delta) override;
		void OnRender(Renderer2D& renderer) override;

		inline const std::vector<Widget*>& getWidgets() const { return m_Widgets; }
	};

} } }