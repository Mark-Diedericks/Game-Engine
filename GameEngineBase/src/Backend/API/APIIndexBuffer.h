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

		virtual void getIndexData32(uint* data) = 0;
		virtual void getIndexData16(uint16* data) = 0;

		static APIIndexBuffer* Create(uint16* data, uint count);
		static APIIndexBuffer* Create(uint* data, uint count);
	};

} } }