#pragma once

#include "Common.h"
#include "Backend/API/APIRenderer.h"
#include "GLContext.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLRenderer : public APIRenderer
	{
	private:
		GLContext* m_Context;
		String m_RendererTitle;
	protected:
		void InitInternal() override;
		void DestroyInternal() override;

		void ClearInternal(uint buffer) override;
		void ClearColorDepthInternal() override;
		void PresentInternal() override;

		void setDepthTestingInternal(bool enabled) override;
		void setBlendInternal(bool enabled) override;
		void setViewportInternal(uint x, uint y, uint width, uint height) override;

		void setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination) override;
		void setBlendEquationInternal(RendererBlendEquation blendEquation) override;

		inline const String& getTitleInternal() const override { return m_RendererTitle; }
	public:
		GLRenderer();
	};

} }