#pragma once

#include "Common.h"
#include "Graphics/Context/Context.h"
#include "Graphics/BackendDependency/IRenderAPIEmployable.h"

namespace gebase { namespace graphics {

	class GE_API IRenderer : public IRenderAPIEmployable
	{
	protected:
		IRenderer() { Context::Add(this); }
	public:
		virtual bool PreEmployRenderAPI() = 0;
	};

} }