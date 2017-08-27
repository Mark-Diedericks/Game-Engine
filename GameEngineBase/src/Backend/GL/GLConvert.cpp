#include "ge.h"
#include "GLConvert.h"

namespace gebase { namespace graphics { namespace API {

	uint GLConvert::RendererBufferToGL(uint buffer)
	{
		uint result = 0;

		if (buffer & RENDERER_BUFFER_COLOR)
			result |= GL_COLOR_BUFFER_BIT;

		if (buffer & RENDERER_BUFFER_DEPTH)
			result |= GL_DEPTH_BUFFER_BIT;

		if (buffer & RENDERER_BUFFER_STENCIL)
			result |= GL_STENCIL_BUFFER_BIT;

		return result;
	}

	uint GLConvert::RendererBlendFunctionToGL(RendererBlendFunction function)
	{
		switch (function)
		{
		case RendererBlendFunction::ZERO:					return GL_ZERO;
		case RendererBlendFunction::ONE:					return GL_ONE;
		case RendererBlendFunction::SOURCE_ALPHA:			return GL_SRC_ALPHA;
		case RendererBlendFunction::DESTINATION_ALPHA:		return GL_DST_ALPHA;
		case RendererBlendFunction::ONE_MINUS_SOURCE_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		}

		return 0;
	}

	uint GLConvert::TextureFormatToGL(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB: return GL_RGB;
		case TextureFormat::RGBA: return GL_RGBA;
		case TextureFormat::LUMINANCE: return GL_LUMINANCE;
		case TextureFormat::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
		}

		return 0;
	}

	uint GLConvert::TextureWrapToGL(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::CLAMP: return GL_CLAMP;
		case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TextureWrap::REPEAT: return GL_REPEAT;
		case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		}

		return 0;
	}

	uint GLConvert::BufferUsageToGL(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::STATIC: return GL_STATIC_DRAW;
		case BufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
		}

		return 0;
	}

} } }