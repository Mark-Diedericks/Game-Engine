#include "ge.h"
#include "APIFramebufferDepth.h"
#include "Graphics/Context.h"

#include "Backend/GL/GLFramebufferDepth.h"
#include "Backend/D3D/11/DX11FramebufferDepth.h"
//#include "Backend/D3D/12/DX12FramebufferDepth.h"
//#include "Backend/VK/VKFramebufferDepth.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIFramebufferDepth* APIFramebufferDepth::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLFramebufferDepth(width, height);
			//case RenderAPI::VULKAN: return genew VKFramebufferDepth(width, height);
		case RenderAPI::D3D11: return genew DX11FramebufferDepth(width, height);
			//case RenderAPI::D3D12: return genew DX12FramebufferDepth(width, height);
		}

		return nullptr;
	}

} } }