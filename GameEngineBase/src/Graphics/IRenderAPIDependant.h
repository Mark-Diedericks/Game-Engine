#pragma once

#include "Common.h"
#include "Context.h"

namespace gebase { namespace graphics {

	enum class RenderObjectType
	{
		Shader = 0,
		Texture,
		Framebuffer,
		Material,
		Buffer
	};

	class GE_API IRenderAPIDependant
	{
	public:
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
		RenderObjectType type;
	protected:
		IRenderAPIDependant(RenderObjectType t) { type = t; }
	};

} }