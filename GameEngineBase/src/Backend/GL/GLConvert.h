#pragma once

#include "GLCommon.h"
#include "Backend/API/APIVertexBuffer.h"
#include "Backend/API/APITexture2D.h"
#include "Backend/API/APIRenderer.h"

namespace gebase { namespace graphics { namespace API {

	class GLConvert
	{
	public:
		static uint RendererBufferToGL(uint buffer);
		static uint RendererBlendFunctionToGL(RendererBlendFunction function);

		static uint TextureFormatToGL(TextureFormat format);
		static uint TextureWrapToGL(TextureWrap wrap);

		static uint BufferUsageToGL(BufferUsage usage);
	};

} } }