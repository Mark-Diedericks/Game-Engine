#include "Pch.h"
#include "APITextureDepth.h"
#include "Graphics/Context.h"

#include "Backend/GL/GLTextureDepth.h"
#include "Backend/D3D/11/DX11TextureDepth.h"
//#include "Backend/D3D/12/DX12TextureDepth.h"
//#include "Backend/VK/VKTextureDepth.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APITextureDepth* APITextureDepth::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureDepth(width, height);
		//case RenderAPI::VULKAN: return genew VKTextureDepth(width, height);
		case RenderAPI::D3D11: return genew DX11TextureDepth(width, height);
		//case RenderAPI::D3D12: return genew DX12TextureDepth(width, height);
		}

		return nullptr;
	}

} } }