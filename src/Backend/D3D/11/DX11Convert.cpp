#include "ge.h"
#include "Common.h"
#include "DX11Convert.h"

namespace gebase { namespace graphics { namespace API {

	uint DX11Convert::RendererBufferToDX(uint buffer) 
	{
		return 0;
	}

	uint DX11Convert::RendererBlendFunctionToDX(RendererBlendFunction function)
	{
		return 0;
	}


	DXGI_FORMAT DX11Convert::TextureFormatToDX(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:
		case TextureFormat::RGBA:
		case TextureFormat::LUMINANCE_ALPHA:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	uint DX11Convert::TextureWrapToDX(TextureWrap wrap)
	{
		return 0;
	}


	uint DX11Convert::BufferUsageToDX(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::STATIC: return D3D11_USAGE_DYNAMIC; //D3D11_USAGE_IMMUTABLE
		case BufferUsage::DYNAMIC: return D3D11_USAGE_DYNAMIC;
		}

		return 0;
	}


} } }