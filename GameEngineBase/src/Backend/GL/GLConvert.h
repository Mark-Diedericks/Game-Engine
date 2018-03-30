#pragma once

#include "GLCommon.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Texture/Texture2D.h"
#include "Graphics/Renderer/Renderer.h"

namespace gebase { namespace graphics {

	class GLConvert
	{
	public:
		static uint RendererBufferToGL(uint buffer);
		static uint RendererBlendFunctionToGL(RendererBlendFunction function);

		static uint TextureFormatToGL(TextureFormat format);
		static uint TextureWrapToGL(TextureWrap wrap);

		static uint BufferUsageToGL(BufferUsage usage);
	};

} }