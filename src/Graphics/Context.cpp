#include "ge.h"
#include "Context.h"

#include "Backend/API/APIContext.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_PreviousRenderAPI = RenderAPI::NONE;
	RenderAPI Context::s_DefaultRenderAPI = RenderAPI::OPENGL;

	void Context::Create(WindowProperties properties, void* deviceContext)
	{
		s_Properties = properties;
		s_DeviceContext = deviceContext;
		API::APIContext::Create(properties, deviceContext);
	}

} }