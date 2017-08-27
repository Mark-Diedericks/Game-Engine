#include "ge.h"
#include "APITextureCube.h"
#include "Graphics/Context.h"

#include "Backend/GL/GLTextureCube.h"
#include "Backend/D3D/11/DX11TextureCube.h"
//#include "Backend/D3D/12/DX12TextureCube.h"
//#include "Backend/VK/VKTextureCube.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APITextureCube* APITextureCube::CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, pixels, width, height, bits);
		//case RenderAPI::VULKAN: return genew VKTextureCube(name, pixels, width, height, bits);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, pixels, width, height, bits);
		//case RenderAPI::D3D12: return genew DX12TextureCube(name, pixels, width, height, bits);
		}

		return nullptr;
	}

	APITextureCube* APITextureCube::CreateFromFiles(const String& name, const byte** sides, uint width, uint height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, sides, width, height, bits);
		//case RenderAPI::VULKAN: return genew VKTextureCube(name, sides, width, height, bits);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, sides, width, height, bits);
		//case RenderAPI::D3D12: return genew DX12TextureCube(name, sides, width, height, bits);
		}

		return nullptr;
	}

	APITextureCube* APITextureCube::CreateFromVerticalCross(const String& name, const byte** sides, int32 mips, uint width, uint height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		//case RenderAPI::VULKAN: return genew VKTextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		//case RenderAPI::D3D12: return genew DX12TextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		}

		return nullptr;
	}

} } }