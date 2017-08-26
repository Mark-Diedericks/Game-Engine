#pragma once

#include "Common.h"
#include "Application/Window.h"
#include "Backend/API/APIContext.h"

namespace gebase { namespace graphics { 

	enum class GE_API RenderAPI
	{
		NONE,
		OPENGL,
		VULKAN,
		D3D11,
		D3D12
	};

	class GE_API Context
	{
	private:
		static RenderAPI s_RenderAPI;
		static RenderAPI s_PreviousRenderAPI;
		static RenderAPI s_DefaultRenderAPI;
		static void* s_DeviceContext;
		static WindowProperties s_Properties;

		Context();
	public:
		static void Create(WindowProperties properties, void* deviceContext);

		static RenderAPI getRenderAPI() { return s_RenderAPI; }
		static void setRenderAPI(RenderAPI api) { s_PreviousRenderAPI = s_RenderAPI;  s_RenderAPI = api; }
		static void revertRenderAPI() { s_RenderAPI = s_PreviousRenderAPI != RenderAPI::NONE ? s_PreviousRenderAPI : s_DefaultRenderAPI; }
	};

} }