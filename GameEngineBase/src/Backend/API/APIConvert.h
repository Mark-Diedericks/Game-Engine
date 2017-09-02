#pragma once

#include "Common.h"

namespace gebase { namespace graphics { namespace API {

	enum class BufferElementType;

	class GE_API APIConvert
	{
	public:
		static uint BufferElementSize(API::BufferElementType type);
		static uint BufferElementType(API::BufferElementType type);

		static uint BufferElementSizeToGL(API::BufferElementType type);
		static uint BufferElementSizeToDX(API::BufferElementType type);

		static uint BufferElementTypeToGL(API::BufferElementType type);
		static uint BufferElementTypeToDX(API::BufferElementType type);
	};

} } }