#include "ge.h"
#include "APIFramebuffer2D.h"
#include "Graphics/Context.h"

#include "Backend/GL/GLFramebuffer2D.h"
#include "Backend/D3D/11/DX11Framebuffer2D.h"
//#include "Backend/D3D/12/DX12Framebuffer2D.h"
//#include "Backend/VK/VKFramebuffer2D.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIFramebuffer2D* APIFramebuffer2D::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLFramebuffer2D(width, height);
		//case RenderAPI::VULKAN: return genew VKFramebuffer2D(width, height);
		case RenderAPI::D3D11: return genew DX11Framebuffer2D(width, height);
		//case RenderAPI::D3D12: return genew DX12Framebuffer2D(width, height);
		}

		return nullptr;
	}

} } }