#pragma once

#include "Common.h"
#include "Types.h"

#include "Events/Event.h"
#include "Events/IEventListener.h"

#include "Application/Window.h"
#include "Utils/TimeUtil.h"

namespace gebase { namespace graphics {

	class GE_API Layer : public events::IEventListener
	{
	protected:
		Window* m_Window;
		bool m_Visible;
		
		virtual bool OnResize(uint width, uint height);
	public:
		Layer();
		virtual ~Layer();

		inline bool isVisible() const { return m_Visible; }
		inline void setVisible(bool visible) { m_Visible = visible; }

		virtual void Init();
		virtual void OnEvent(events::Event& event);
		virtual void OnTick();
		virtual void OnUpdate(const float delta);
		virtual void OnUpdateInternal(const float delta);
		virtual void OnRender();
	};

} }