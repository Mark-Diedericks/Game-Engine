#pragma once

#include "DX11Common.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Texture/Texture2D.h"
#include "Backend/API/APIRenderer.h"

namespace gebase { namespace graphics {

	class DX11Convert
	{
	public:
		static uint RendererBufferToDX(uint buffer);
		static uint RendererBlendFunctionToDX(RendererBlendFunction function);

		static DXGI_FORMAT TextureFormatToDX(TextureFormat format);
		static uint TextureWrapToDX(TextureWrap wrap);

		static uint BufferUsageToDX(BufferUsage usage);
	};

} }