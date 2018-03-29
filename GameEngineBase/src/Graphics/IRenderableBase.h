#pragma once

#include "Common.h"
#include "Context.h"
#include "IRenderAPIEmployable.h"

namespace gebase { namespace graphics {

	class GE_API IRenderableBase : public IRenderAPIEmployable
	{
	protected:
		IRenderableBase() { Context::Add(this); }
	};

} }