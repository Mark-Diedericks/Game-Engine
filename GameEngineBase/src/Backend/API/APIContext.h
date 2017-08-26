#pragma once

#include "Common.h"
#include "Application/Window.h"

namespace gebase { namespace graphics { namespace API {

	class APIContext
	{
	protected:
		static API::APIContext* s_Context;
	public:
		static void Create(WindowProperties properties, void* deviceContext);
	};

} } }