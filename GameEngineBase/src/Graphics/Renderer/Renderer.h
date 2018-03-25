#pragma once

#include "Common.h"
#include "CustomString.h"
#include "Backend/API/APIRenderer.h"

namespace gebase { namespace graphics {

	enum class RenderAPI;

	class GE_API Renderer
	{
	private:
		static Renderer* s_Instance;
		Renderer() { }
	public:
		static void Init();

		static bool EmployRenderAPI(RenderAPI api);

		inline static void Clear(uint buffer) { APIRenderer::Clear(buffer); }
		inline static void ClearColorDepth() { APIRenderer::ClearColorDepth(); }
		inline static void Present() { APIRenderer::Present(); }

		inline static void setDepthTesting(bool enabled) { APIRenderer::setDepthTesting(enabled); }
		inline static void setBlend(bool enabled) { APIRenderer::setBlend(enabled); }
		inline static void setViewport(uint x, uint y, uint width, uint height) { APIRenderer::setViewport(x, y, width, height); }

		inline static void setBlendFunction(RendererBlendFunction source, RendererBlendFunction destination) { APIRenderer::setBlendFunction(source, destination); }
		inline static void setBlendEquation(RendererBlendEquation blendEquation) { APIRenderer::setBlendEquation(blendEquation); }

		inline static const String& getTitle() { return APIRenderer::getTitle(); }
		inline static APIRenderer* getRenderer() { return APIRenderer::getRenderer(); }
	};

} }