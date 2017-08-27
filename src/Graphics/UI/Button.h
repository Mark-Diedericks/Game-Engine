#pragma once

#include "Common.h"
#include "String.h"
#include "Widget.h"

namespace gebase { namespace graphics { namespace ui {

	class Button : public Widget
	{
	public:
		using ActionHandler = std::function<void()>;
	private:
		enum class ButtonState
		{
			UNPRESSED,
			PRESSED
		};

		static void NoAction() {}
	protected:
		String m_Label;
		Font* m_Font;
		ButtonState m_State;
		ActionHandler m_ActionHandler;
	public:
		Button(const String& label, const math::Rectangle& bounds, const ActionHandler& hander = &Button::NoAction);

		virtual void OnAction();
		virtual void OnUpdate(float delta) override;
		virtual void OnRender(Renderer2D& renderer) override;

		bool OnMousePressed(events::MousePressedEvent& e) override;
		bool OnMouseReleased(events::MouseReleasedEvent& e) override;
		bool OnMouseMoved(events::MouseMovedEvent& e) override;

		inline void setLabel(const String& label) { m_Label = label; }
		inline const String& getLabel() const { return m_Label; }

		inline void setFont(Font* font) { m_Font = font; }
		inline const Font& getFont() const { return *m_Font; }

		inline void setAction(const ActionHandler& action) { m_ActionHandler = action; }
	};

} } }