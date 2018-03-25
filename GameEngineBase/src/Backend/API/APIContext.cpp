#include "ge.h"
#include "APIContext.h"

#include "Backend/GL/GLContext.h"
#include "Backend/D3D/11/DX11Context.h"
//#include "Backend/D3D/12/D12Context.h"
//#include "Backend/VK/VKContext.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	APIContext* APIContext::s_Context = nullptr;

	void APIContext::Create(WindowProperties properties, void* deviceContext)
	{
		if (s_Context != nullptr)
			gedel s_Context;

		s_Context = nullptr;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: s_Context = genew GLContext(properties, deviceContext);
			break;
		//case RenderAPI::VULKAN: s_Context = genew VKContext(properties, deviceContext);
		//	break;
		case RenderAPI::D3D11: s_Context = genew DX11Context(properties, deviceContext);
			break;
		//case RenderAPI::D3D12: s_Context = genew DX12Context(properties, deviceContext);
		//	break;
		}
	}

} }