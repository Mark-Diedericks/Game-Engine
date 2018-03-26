#pragma once
#include "Backend/API/APIContext.h"
#include "DX11Common.h"

namespace gebase { namespace graphics {

	class DX11Context : public APIContext
	{
	private:
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		uint m_MSAAQuality;
		bool m_MSAAEnabled;
		bool m_DebugLayerEnabled;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		D3D11_VIEWPORT m_ScreenViewport;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11Debug* m_DebugLayer;

		WindowProperties m_Properties;

		void InitD3D(HWND hWnd);
		void Resize();

		void setRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);
		String getD3DVersionStringInternal() const;
	protected:
		void DestroyInternal() override;
	public:
		IDXGISwapChain* swapchain;
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		ID3D11RenderTargetView* backbuffer;

		DX11Context(WindowProperties properties, void* deviceContext);
		~DX11Context();

		void Present();

		inline static DX11Context* Get() { return (DX11Context*)s_Context; }
		inline static String getDXVersionString() { return Get()->getD3DVersionStringInternal(); }

		inline static ID3D11DepthStencilView* getDepthStencilBuffer() { return Get()->m_DepthStencilView; }

		inline static IDXGISwapChain* getSwapChain() { return Get()->swapchain; }
		inline static ID3D11Device* getDevice() { return Get()->device; }
		inline static ID3D11DeviceContext* getDeviceContext() { return Get()->deviceContext; }
		inline static ID3D11RenderTargetView* getBackBuffer() { return Get()->m_RenderTargetView; }
	};

} }