#pragma once

#include "Graphics/Renderer/Renderer.h"
#include "DX11Context.h"

namespace gebase { namespace graphics {
	
	class DX11Renderer : public Renderer
	{
	private:
		static std::vector<ID3D11BlendState*> s_BlendStates;
		static std::vector<ID3D11DepthStencilState*> s_DepthStencilStates;

		DX11Context* m_Context;
		String m_RendererTitle;

		void CreateBlendStates();
		void CreateDepthStencilStates();
	protected:
		void InitInternal() override;
		void DestroyInternal() override;

		void ClearInternal(uint buffer) override;
		void ClearColorDepthInternal() override;

		void setDepthTestingInternal(bool enabled) override;
		void setBlendInternal(bool enabled) override;
		
		void setViewportInternal(uint x, uint y, uint width, uint height) override;

		void setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination) override;
		void setBlendEquationInternal(RendererBlendEquation blendEquation) override;

		inline const String& getTitleInternal() const override { return m_RendererTitle; }
	public:
		DX11Renderer();
	};

} }