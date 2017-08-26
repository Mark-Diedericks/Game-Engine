#include "Pch.h"
#include "Common.h"
#include "VertexBuffer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	VertexBuffer* VertexBuffer::Create(API::BufferUsage usage)
	{
		VertexBuffer* thisVB = genew VertexBuffer();
		thisVB->m_Instance = API::APIVertexBuffer::Create(usage);
		return thisVB;
	}

	void VertexBuffer::setData(uint size, const void* data)
	{
		m_Instance->setData(size, data);
		m_Data = data;
	}

} }