#pragma once

#include "Common.h"
#include "Graphics/Context/Context.h"
#include "Graphics/BackendDependency/IRenderAPIEmployable.h"

namespace gebase { namespace graphics {

	class GE_API IRenderableBase : public IRenderAPIEmployable
	{
	protected:
		IRenderableBase() { Context::Add(this); }
	};

} }