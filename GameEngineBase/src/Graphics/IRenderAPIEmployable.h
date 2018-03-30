#pragma once

#include "Common.h"
#include "Context.h"

namespace gebase { namespace graphics {

	class GE_API IRenderAPIEmployable
	{
	public:
		virtual bool EmployRenderAPIShader(RenderAPI api) = 0;

		virtual bool EmployRenderAPITexture2D(RenderAPI api) = 0;
		virtual bool EmployRenderAPITextureCube(RenderAPI api) = 0;
		virtual bool EmployRenderAPITextureDepth(RenderAPI api) = 0;

		virtual bool EmployRenderAPIFramebuffer2D(RenderAPI api) = 0;
		virtual bool EmployRenderAPIFramebufferDepth(RenderAPI api) = 0;

		virtual bool EmployRenderAPIIndexBuffer(RenderAPI api) = 0;
		virtual bool EmployRenderAPIVertexArray(RenderAPI api) = 0;
	};

} }
