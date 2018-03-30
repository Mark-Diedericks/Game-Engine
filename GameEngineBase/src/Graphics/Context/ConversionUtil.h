#pragma once

#include "Common.h"
#include "Graphics/Buffer/BufferLayout.h"
#include "Graphics/Context/Context.h"

namespace gebase { namespace graphics {

	enum class BufferElementType;

	class GE_API ConversionUtil
	{
	public:
		static uint GetBufferElementSize(BufferElementType type, RenderAPI api = Context::getRenderAPI());
		static uint GetBufferElementType(BufferElementType type, RenderAPI api = Context::getRenderAPI());

		static uint BufferElementSizeToGL(BufferElementType type);
		static uint BufferElementSizeToDX(BufferElementType type);

		static uint BufferElementTypeToGL(BufferElementType type);
		static uint BufferElementTypeToDX(BufferElementType type);
	};

} }