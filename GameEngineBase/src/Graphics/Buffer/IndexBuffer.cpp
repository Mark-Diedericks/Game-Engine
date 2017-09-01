#include "ge.h"
#include "Common.h"
#include "IndexBuffer.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	IndexBuffer* IndexBuffer::Create(uint16* data, uint count)
	{
		IndexBuffer* thisIB = genew IndexBuffer();
		thisIB->m_Data16 = data;
		thisIB->m_Count = count;
		thisIB->m_Instance = API::APIIndexBuffer::Create(data, count);
		return thisIB;
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count)
	{
		IndexBuffer* thisIB = genew IndexBuffer();
		thisIB->m_Data = data;
		thisIB->m_Count = count;
		thisIB->m_Instance = API::APIIndexBuffer::Create(data, count);
		return thisIB;
	}

	bool IndexBuffer::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;

		if(m_Data) this->m_Instance = API::APIIndexBuffer::Create(m_Data, m_Count);
		else this->m_Instance = API::APIIndexBuffer::Create(m_Data16, m_Count);

		return true;
	}

} }