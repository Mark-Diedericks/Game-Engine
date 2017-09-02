#pragma once

#include "Common.h"
#include "Context.h"

namespace gebase { namespace graphics {

	class GE_API IRenderableBase
	{
	protected:
		IRenderableBase() { Context::Add(this); }
	public:
		virtual bool PreEmployRenderAPI() = 0;
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
	};

} }