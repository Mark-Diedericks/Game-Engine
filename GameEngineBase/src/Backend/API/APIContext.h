#pragma once

#include "Common.h"
#include "Application/Window.h"

namespace gebase { namespace graphics {

	class APIContext
	{
	protected:
		static APIContext* s_Context;
	public:
		static void Create(WindowProperties properties, void* deviceContext);
	};

} }