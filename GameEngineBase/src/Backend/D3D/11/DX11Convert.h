#pragma once

#include "DX11Common.h"
#include "Backend/API/APIVertexBuffer.h"
#include "Backend/API/APITexture2D.h"
#include "Backend/API/APIRenderer.h"

namespace gebase { namespace graphics { namespace API {

	class DX11Convert
	{
	public:
		static uint RendererBufferToDX(uint buffer);
		static uint RendererBlendFunctionToDX(RendererBlendFunction function);

		static DXGI_FORMAT TextureFormatToDX(TextureFormat format);
		static uint TextureWrapToDX(TextureWrap wrap);

		static uint BufferUsageToDX(BufferUsage usage);
	};

} } }