#pragma once

#include "Common.h"

namespace gebase { namespace graphics {

	enum class RenderAPI;

	class GE_API IRenderAPIDependant
	{
	public:
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
	};

} }