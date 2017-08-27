#pragma once

#include "EventHeader.h"
#include <functional>

namespace gebase { namespace events {

	class GE_API EventDispatcher
	{
	private:
		Event& m_Event;
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T>
		void Dispatch(std::function<bool(T&)> f)
		{
			if ((int32)m_Event.getEventType() & (int32)T::getStaticType())
				m_Event.m_Handled = f(*(T*)&m_Event);
		}
	};

} }