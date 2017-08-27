#pragma once

#include "Common.h"
#include "Widget.h"

namespace gebase { namespace graphics { namespace ui {

	class GE_API Slider : public Widget
	{
	public:
		using ValueChangedCallback = std::function<void(float)>;
	private:
		enum class SliderState
		{
			UNPRESSED,
			PRESSED,
			PRESSEDHEAD
		};

		math::Rectangle m_HeadBounds;
		
		float m_Value;
		float m_HeadOffset;
		bool m_Vertical;

		SliderState m_State;
		ValueChangedCallback m_Callback;

		static void NoCallback(float) {}
	public:
		Slider(const math::Rectangle& bounds, bool vertical = false);
		Slider(const math::Rectangle& bounds, float value = 0.0f, const ValueChangedCallback& callback = &Slider::NoCallback, bool vertical = false);

		void OnUpdate(float delta) override;
		void OnRender(Renderer2D& renderer) override;

		bool OnMousePressed(events::MousePressedEvent& e) override;
		bool OnMouseReleased(events::MouseReleasedEvent& e) override;
		bool OnMouseMoved(events::MouseMovedEvent& e) override;

		inline void setCallback(const ValueChangedCallback& callback) { m_Callback = callback; }
		inline const ValueChangedCallback& getCallback() const { return m_Callback; }

		void setValue(float value);
		inline float getValue() const { return m_Value; }
	};

} } }