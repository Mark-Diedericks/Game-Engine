#include "ge.h"
#include "Common.h"
#include "DX11Context.h"

#include <Windows.h>
#include <d3d11.h>

namespace gebase { namespace graphics {

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}

	DX11Context* DX11Context::s_Instance = nullptr;

#ifdef GE_DEBUG
	DX11Context::DX11Context(WindowProperties properties, void* deviceContext) : m_DebugLayerEnabled(true)
#else
	DX11Context::DX11Context(WindowProperties properties, void* deviceContext) : m_DebugLayerEnabled(false)
#endif
	{
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilBuffer = nullptr;

		s_Instance = this;

		m_Properties = properties;
		InitD3D((HWND)deviceContext);
	}

	DX11Context::~DX11Context()
	{

	}

	void DX11Context::InitD3D(HWND hWnd)
	{
		uint width = m_Properties.width;
		uint height = m_Properties.height;
		m_MSAAEnabled = true; //TODO Make a setting

		HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, m_DebugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, NULL, D3D11_SDK_VERSION, &device, &m_D3DFeatureLevel, &deviceContext);
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 60; //TODO Make a setting
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 3;
		scd.OutputWindow = hWnd;
		scd.Windowed = !m_Properties.fullscreen;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory* dxgiFactory = nullptr;

		device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		dxgiFactory->CreateSwapChain(device, &scd, &swapchain);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		if (m_DebugLayerEnabled)
		{
			device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_DebugLayer));
			m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

			ID3D11InfoQueue* infoQueue;
			device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));

			D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
			D3D11_INFO_QUEUE_FILTER filter;

			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = 1;
			filter.DenyList.pIDList = hide;
			infoQueue->AddStorageFilterEntries(&filter);
		}

		Resize();
	}

	void DX11Context::Resize()
	{
		uint width = m_Properties.width;
		uint height = m_Properties.height;

		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ID3D11Texture2D *backBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		backBuffer->Release();

		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width = width;
		dsd.Height = height;
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		dsd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		dsd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;

		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		device->CreateTexture2D(&dsd, 0, &m_DepthStencilBuffer);
		device->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView);
		setRenderTargets(m_RenderTargetView, m_DepthStencilView);

		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;

		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;

		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		
		deviceContext->RSSetViewports(1, &m_ScreenViewport);

		D3D11_RASTERIZER_DESC rd;
		rd.AntialiasedLineEnable = false;
		rd.CullMode = D3D11_CULL_NONE;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.DepthClipEnable = true;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.FrontCounterClockwise = true;
		rd.MultisampleEnable = false;
		rd.ScissorEnable = false;
		rd.SlopeScaledDepthBias = 0.0f;

		ID3D11RasterizerState* rs;
		device->CreateRasterizerState(&rd, &rs);
		deviceContext->RSSetState(rs);

		ReleaseCOM(rs);
	}

	void DX11Context::setRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view)
	{
		deviceContext->OMSetRenderTargets(1, &target, view);
	}

	void DX11Context::PresentInternal()
	{
		swapchain->Present(m_Properties.vsync, 0);
	}

	void DX11Context::DestroyInternal()
	{
		if (swapchain)
			swapchain->Release();

		if (deviceContext)
			deviceContext->Release();

		if(device)
			device->Release();
	}

	String DX11Context::getD3DVersionStringInternal() const
	{
		switch (m_D3DFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_0: return "11";
		case D3D_FEATURE_LEVEL_10_1: return "10.1";
		case D3D_FEATURE_LEVEL_10_0: return "10";
		}

		return "Unknown version.";
	}

} }