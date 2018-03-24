#pragma once

#include "Common.h"
#include "CustomString.h"

namespace gebase { namespace graphics { namespace API {

	enum GE_API RendererBufferType
	{
		RENDERER_BUFFER_NONE = 0,
		RENDERER_BUFFER_COLOR = BIT(0),
		RENDERER_BUFFER_DEPTH = BIT(1),
		RENDERER_BUFFER_STENCIL = BIT(2)
	};

	enum class GE_API RendererBlendFunction
	{
		NONE,
		ZERO,
		ONE,
		SOURCE_ALPHA,
		DESTINATION_ALPHA,
		ONE_MINUS_SOURCE_ALPHA
	};

	enum class GE_API RendererBlendEquation
	{
		NONE,
		ADD,
		SUBTRACT
	};

	class APIRenderer
	{
	private:
		static APIRenderer* s_Instance;
	protected:
		virtual void InitInternal() = 0;

		virtual void ClearInternal(uint buffer) = 0;
		virtual void ClearColorDepthInternal() = 0;
		virtual void PresentInternal() = 0;

		virtual void setDepthTestingInternal(bool enabled) = 0;
		virtual void setBlendInternal(bool enabled) = 0;
		virtual void setViewportInternal(uint x, uint, uint width, uint height) = 0;

		virtual void setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination) = 0;
		virtual void setBlendEquationInternal(RendererBlendEquation blendEquation) = 0;

		virtual const String& getTitleInternal() const = 0;
	public:
		static void Init();

		inline static void Clear(uint buffer) { s_Instance->ClearInternal(buffer); }
		inline static void ClearColorDepth() { s_Instance->ClearColorDepthInternal(); }
		inline static void Present() { s_Instance->PresentInternal(); }

		inline static void setDepthTesting(bool enabled) { s_Instance->setDepthTestingInternal(enabled); }
		inline static void setBlend(bool enabled) { s_Instance->setBlendInternal(enabled); }
		inline static void setViewport(uint x, uint y, uint width, uint height) { s_Instance->setViewportInternal(x, y, width, height); }

		inline static void setBlendFunction(RendererBlendFunction source, RendererBlendFunction destination) { s_Instance->setBlendFunctionInternal(source, destination); }
		inline static void setBlendEquation(RendererBlendEquation blendEquation) { s_Instance->setBlendEquationInternal(blendEquation); }

		inline static const String& getTitle() { return s_Instance->getTitleInternal(); }
		inline static APIRenderer* getRenderer() { return s_Instance; }
	};

} } }