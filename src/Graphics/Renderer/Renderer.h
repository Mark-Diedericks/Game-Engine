#pragma once

#include "Common.h"
#include "String.h"
#include "Backend/API/APIRenderer.h"

namespace gebase { namespace graphics {

	class Renderer
	{
	private:
		static Renderer* s_Instance;
		Renderer();
	public:
		static void Init();

		inline static void Clear(uint buffer) { API::APIRenderer::Clear(buffer); }
		inline static void Present() { API::APIRenderer::Present(); }

		inline static void setDepthTesting(bool enabled) { API::APIRenderer::setDepthTesting(enabled); }
		inline static void setBlend(bool enabled) { API::APIRenderer::setBlend(enabled); }
		inline static void setViewport(uint x, uint y, uint width, uint height) { API::APIRenderer::setViewport(x, y, width, height); }

		inline static void setBlendFunction(API::RendererBlendFunction source, API::RendererBlendFunction destination) { API::APIRenderer::setBlendFunction(source, destination); }
		inline static void setBlendEquation(API::RendererBlendEquation blendEquation) { API::APIRenderer::setBlendEquation(blendEquation); }

		inline static const String& getTitle() { return API::APIRenderer::getTitle(); }
		inline static API::APIRenderer* getRenderer() { return API::APIRenderer::getRenderer(); }
	};

} }