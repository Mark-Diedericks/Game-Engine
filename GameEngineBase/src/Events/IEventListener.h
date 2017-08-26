#pragma once

#include "Event.h"

namespace gebase { namespace events {

	class GE_API IEventListener {
	public:
		virtual void OnEvent(Event& event) = 0;
	};

} }