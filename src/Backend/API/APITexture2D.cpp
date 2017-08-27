#include "ge.h"
#include "APITexture2D.h"
#include "Graphics/Context.h"

#include "Backend/GL/GLTexture2D.h"
#include "Backend/D3D/11/DX11Texture2D.h"
//#include "Backend/D3D/12/DX12Texture2D.h"
//#include "Backend/VK/VKTexture2D.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APITexture2D* APITexture2D::Create(uint width, uint height, TextureParameters parameters)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTexture2D(width, height, parameters);
		//case RenderAPI::VULKAN: return genew VKTexture2D(width, height, parameters);
		case RenderAPI::D3D11: return genew DX11Texture2D(width, height, parameters);
		//case RenderAPI::D3D12: return genew DX12Texture2D(width, height, parameters);
		}

		return nullptr;
	}

	APITexture2D* APITexture2D::CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTexture2D(name, pixels, width, height, bits,  parameters);
		//case RenderAPI::VULKAN: return genew VKTexture2D(name, pixels, width, height, bits, parameters);
		case RenderAPI::D3D11: return genew DX11Texture2D(name, pixels, width, height, bits, parameters);
		//case RenderAPI::D3D12: return genew DX12Texture2D(name, pixels, width, height, bits, parameters);
		}

		return nullptr;
	}

} } }