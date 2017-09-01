#include "ge.h"
#include "Context.h"

#include "Backend/API/APIContext.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_PreviousRenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_DefaultRenderAPI = RenderAPI::OPENGL;

	void* Context::s_DeviceContext = nullptr;
	WindowProperties Context::s_Properties;
	std::vector<IRenderAPIDependant*> Context::s_RenderAPIDependantObjects;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		s_Properties = properties;
		s_DeviceContext = deviceContext;
		API::APIContext::Create(properties, deviceContext);
	}

	bool Context::EmployRenderAPI(RenderAPI api)
	{
		setRenderAPI(api);

		for (uint i = 0; i < s_RenderAPIDependantObjects.size(); i++)
			if(!s_RenderAPIDependantObjects[i]->EmployRenderAPI(api))
				return false;

		return true;
	}

	bool Context::RevertRenderAPI()
	{
		return EmployRenderAPI(s_PreviousRenderAPI != RenderAPI::NONE ? s_PreviousRenderAPI : s_DefaultRenderAPI);
	}

	void Context::Add(IRenderAPIDependant* object)
	{
		s_RenderAPIDependantObjects.push_back(object);
	}

	void Context::Remove(IRenderAPIDependant* object)
	{
		for (uint i = 0; i < s_RenderAPIDependantObjects.size(); i++)
			if (s_RenderAPIDependantObjects[i] == object)
				s_RenderAPIDependantObjects.erase(s_RenderAPIDependantObjects.begin() + i);
	}

} }