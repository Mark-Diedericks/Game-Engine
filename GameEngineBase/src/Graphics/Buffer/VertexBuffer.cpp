#include "ge.h"
#include "Common.h"
#include "VertexBuffer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	VertexBuffer* VertexBuffer::Create(API::BufferUsage usage)
	{
		VertexBuffer* thisVB = genew VertexBuffer();
		thisVB->m_Usage = usage;
		thisVB->m_Instance = API::APIVertexBuffer::Create(usage);
		return thisVB;
	}

	void VertexBuffer::setData(uint size, const void* data)
	{
		m_Instance->setData(size, data);
		m_Data = data;
		m_Size = size;
	}

	bool VertexBuffer::EmployRenderAPI(RenderAPI api)
	{
		m_Layout.EmployRenderAPI(api);
		gedel this->m_Instance;
		this->m_Instance = API::APIVertexBuffer::Create(m_Usage);
		this->m_Instance->setData(m_Size, m_Data);
		this->m_Instance->setLayout(m_Layout)
		return true;
	}

} }