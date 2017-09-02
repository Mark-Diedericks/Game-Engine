#pragma once

#include "ge.h"
#include "Common.h"
#include "APIBufferLayout.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC
	};

	class APIVertexBuffer
	{
	protected:
		virtual void* getPointerInternal() = 0;
	public:
		virtual void Resize(uint size) = 0;
		virtual void setLayout(const APIBufferLayout& layout) = 0;
		virtual void setData(uint size, const void* data) = 0;

		virtual void ReleasePointer() = 0;

		virtual void* getBufferData() = 0;
		virtual uint getSize() = 0;
		virtual APIBufferLayout& getBufferLayout() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		template <typename T>
		T* getPointer()
		{
			return (T*)getPointerInternal();
		}

		static APIVertexBuffer* Create(BufferUsage = BufferUsage::STATIC);
	};

} } }