#pragma once

#include "Common.h"
#include "Application/Window.h"
#include "Backend/API/APIContext.h"
namespace gebase { namespace graphics { 

	class IRenderAPIDependant;
	class IRenderer;
	class IRenderableBase;

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

		static std::vector<IRenderer*> s_RendererObjects;
		static std::vector<IRenderableBase*> s_RenderableObjects;

		Context();
	public:
		static void Create(WindowProperties properties, void* deviceContext);

		static RenderAPI getRenderAPI() { return s_RenderAPI; }
		static void setRenderAPI(RenderAPI api) { s_PreviousRenderAPI = s_RenderAPI;  s_RenderAPI = api; }

		static bool EmployRenderAPI(RenderAPI api);
		static bool RevertRenderAPI();
		static void FlushRenderAPIChange(RenderAPI prevApi);

		static void Add(IRenderer* object);
		static void Remove(IRenderer* object);

		static void Add(IRenderableBase* object);
		static void Remove(IRenderableBase* object);
	};

} }