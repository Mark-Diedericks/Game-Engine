#include "ge.h"
#include "Context.h"
#include "Renderer/Renderer.h"
#include "Backend/API/APIContext.h"
#include "IRenderableBase.h"
#include "Renderer/IRenderer.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_PreviousRenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_DefaultRenderAPI = RenderAPI::OPENGL;

	void* Context::s_DeviceContext = nullptr;
	WindowProperties Context::s_Properties;

	std::vector<IRenderer*> Context::s_RendererObjects;
	std::vector<IRenderableBase*> Context::s_RenderableObjects;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		s_Properties = properties;
		s_DeviceContext = deviceContext;
		APIContext::Create(properties, deviceContext);
	}

	bool Context::EmployRenderAPI(RenderAPI api)
	{
		setRenderAPI(api);

		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if (!s_RendererObjects[i]->PreEmployRenderAPI())
				return false;

		APIContext::Create(s_Properties, s_DeviceContext);

		if (!Renderer::EmployRenderAPI(api))
			return false;

		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if(s_RendererObjects[i])
			if (!s_RendererObjects[i]->EmployRenderAPI(api))
				return false;

		for (uint i = 0; i < s_RenderableObjects.size(); i++)
			if(s_RenderableObjects[i])
				if (!s_RenderableObjects[i]->EmployRenderAPI(api))
					return false;

		return true;
	}

	bool Context::RevertRenderAPI()
	{
		return EmployRenderAPI(s_PreviousRenderAPI != RenderAPI::NONE ? s_PreviousRenderAPI : s_DefaultRenderAPI);
	}

	void Context::Add(IRenderer* object)
	{
		s_RendererObjects.push_back(object);
	}

	void Context::Remove(IRenderer* object)
	{
		for (uint i = 0; i < s_RendererObjects.size(); i++)
			if (s_RendererObjects[i] == object)
				s_RendererObjects.erase(s_RendererObjects.begin() + i);
	}

	void Context::Add(IRenderableBase* object)
	{
		s_RenderableObjects.push_back(object);
	}

	void Context::Remove(IRenderableBase* object)
	{
		for (uint i = 0; i < s_RenderableObjects.size(); i++)
			if (s_RenderableObjects[i] == object)
				s_RenderableObjects.erase(s_RenderableObjects.begin() + i);
	}

} }