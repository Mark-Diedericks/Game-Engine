#pragma once

#include "Common.h"
#include "Graphics/Context.h"

namespace gebase { namespace graphics {

	class GE_API IRenderer
	{
	protected:
		IRenderer() { Context::Add(this); }
	public:
		virtual bool PreEmployRenderAPI() = 0;
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
	};

} }