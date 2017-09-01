#pragma once

#include "Common.h"
#include "Application/Window.h"
#include "Backend/API/APIContext.h"
#include "IRenderAPIDependant.h"

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
		static std::vector<IRenderAPIDependant*> s_RenderAPIDependantObjects;

		Context();

		static void setRenderAPI(RenderAPI api) { s_PreviousRenderAPI = s_RenderAPI;  s_RenderAPI = api; }
	public:
		static void Create(WindowProperties properties, void* deviceContext);

		static RenderAPI getRenderAPI() { return s_RenderAPI; }

		static bool EmployRenderAPI(RenderAPI api);
		static bool RevertRenderAPI();

		static void Add(IRenderAPIDependant* object);
		static void Remove(IRenderAPIDependant* object);
	};

} }