#pragma once

#include "Common.h"
#include "Types.h"
#include "APIVertexBuffer.h"

namespace gebase { namespace graphics { namespace API {

	class APIIndexBuffer
	{
	public:
		virtual uint getCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint* getIndexData32() = 0;
		virtual uint16* getIndexData16() = 0;

		static APIIndexBuffer* Create(uint16* data, uint count);
		static APIIndexBuffer* Create(uint* data, uint count);
	};

} } }