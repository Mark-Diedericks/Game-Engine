#include "ge.h"
#include "Common.h"
#include "DX11Renderer.h"
#include "DX11Common.h"
#include "System/MemoryManager.h"

namespace gebase { namespace graphics {

	std::vector<ID3D11BlendState*> DX11Renderer::s_BlendStates;
	std::vector<ID3D11DepthStencilState*> DX11Renderer::s_DepthStencilStates;

	DX11Renderer::DX11Renderer()
	{
		m_Context = DX11Context::Get();
	}

	void DX11Renderer::CreateBlendStates()
	{
		{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
			desc.RenderTarget[0].BlendEnable = false;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			ID3D11BlendState* state;
			DXCall(DX11Context::getDevice()->CreateBlendState(&desc, &state));
			s_BlendStates.push_back(state);
		}
		{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
			desc.AlphaToCoverageEnable = false;
			desc.IndependentBlendEnable = false;

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			ID3D11BlendState* state;
			DXCall(DX11Context::getDevice()->CreateBlendState(&desc, &state));
			s_BlendStates.push_back(state);
		}
	}

	void DX11Renderer::CreateDepthStencilStates()
	{
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.StencilEnable = true;
			desc.StencilReadMask = 0xff;
			desc.StencilWriteMask = 0xff;

			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

			ID3D11DepthStencilState* state;
			DXCall(DX11Context::getDevice()->CreateDepthStencilState(&desc, &state));
			s_DepthStencilStates.push_back(state);
		}
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			desc.StencilEnable = true;
			desc.StencilReadMask = 0xff;
			desc.StencilWriteMask = 0xff;

			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

			ID3D11DepthStencilState* state;
			DXCall(DX11Context::getDevice()->CreateDepthStencilState(&desc, &state));
			s_DepthStencilStates.push_back(state);
		}
	}

	void DX11Renderer::InitInternal()
	{
		CreateBlendStates();
		CreateDepthStencilStates();

		setDepthTesting(true);
		setBlendFunction(RendererBlendFunction::SOURCE_ALPHA, RendererBlendFunction::ONE_MINUS_SOURCE_ALPHA);
		setBlend(true);

		IDXGIDevice* dxgiDevice;
		DX11Context::getDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

		IDXGIAdapter* adapter;
		dxgiDevice->GetAdapter(&adapter);

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		utils::LogUtil::WriteLine("INFO", "-----------------------------------");
		utils::LogUtil::WriteLine("INFO", "              DIRECT3D             ");
		utils::LogUtil::WriteLine("INFO", "		Direct3D " + DX11Context::getDXVersionString());
		utils::LogUtil::WriteLine("INFO", "		DESC: " + (String)(char*)(desc.Description));
		utils::LogUtil::WriteLine("INFO", "		VRAM: " + MemoryManager::BytesToString(desc.DedicatedVideoMemory));
		utils::LogUtil::WriteLine("INFO", "-----------------------------------");

		m_RendererTitle = "Direct3D " + DX11Context::getDXVersionString();
	}

	void DX11Renderer::DestroyInternal()
	{

	}

	void DX11Renderer::ClearInternal(uint buffer)
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		if (buffer & RendererBufferType::RENDERER_BUFFER_COLOR)
			DX11Context::getDeviceContext()->ClearRenderTargetView(DX11Context::getBackBuffer(), color);

		if (buffer & RendererBufferType::RENDERER_BUFFER_DEPTH)
			DX11Context::getDeviceContext()->ClearDepthStencilView(DX11Context::getDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Renderer::ClearColorDepthInternal()
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		DX11Context::getDeviceContext()->ClearRenderTargetView(DX11Context::getBackBuffer(), color);
		DX11Context::getDeviceContext()->ClearDepthStencilView(DX11Context::getDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Renderer::PresentInternal()
	{
		m_Context->Present();
	}

	void DX11Renderer::setDepthTestingInternal(bool enabled)
	{
		DX11Context::getDeviceContext()->OMSetDepthStencilState(enabled ? s_DepthStencilStates[0] : s_DepthStencilStates[1], NULL);
	}

	void DX11Renderer::setBlendInternal(bool enabled)
	{
		DX11Context::getDeviceContext()->OMSetBlendState(enabled ? s_BlendStates[1] : s_BlendStates[0], NULL, 0xffffffff);
	}

	void DX11Renderer::setViewportInternal(uint x, uint y, uint width, uint height)
	{

	}

	void DX11Renderer::setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination)
	{

	}

	void DX11Renderer::setBlendEquationInternal(RendererBlendEquation blendEquation)
	{

	}

} } 