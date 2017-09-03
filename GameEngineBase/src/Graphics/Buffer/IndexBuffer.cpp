#include "ge.h"
#include "Common.h"
#include "IndexBuffer.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	IndexBuffer* IndexBuffer::Create(uint16* data, uint count)
	{
		IndexBuffer* thisIB = genew IndexBuffer();
		thisIB->m_Type = 0;
		thisIB->m_Count = count;
		thisIB->m_Instance = API::APIIndexBuffer::Create(data, count);
		return thisIB;
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count)
	{
		IndexBuffer* thisIB = genew IndexBuffer();
		thisIB->m_Type = 1;
		thisIB->m_Count = count;
		thisIB->m_Instance = API::APIIndexBuffer::Create(data, count);
		return thisIB;
	}

	bool IndexBuffer::EmployRenderAPI(RenderAPI api)
	{
		if (current == api)
			return true;

		current = api;

		API::APIIndexBuffer* inst;

		if (m_Type) 
		{
			uint* data = genew uint[m_Count];
			this->m_Instance->getIndexData32(data);

			inst = API::APIIndexBuffer::Create(data, m_Count);

			//if(data)
			//	gedel[] data;

			gedel this->m_Instance;
		}
		else
		{
			uint16* data = genew uint16[m_Count];
			this->m_Instance->getIndexData16(data);

			inst = API::APIIndexBuffer::Create(data, m_Count);

			//if(data)
			//	gedel[] data;

			gedel this->m_Instance;
		}

		if (!inst)
			return false;

		this->m_Instance = inst;

		return true;
	}

} }