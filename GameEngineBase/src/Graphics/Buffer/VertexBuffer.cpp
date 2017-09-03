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
	}

	bool VertexBuffer::EmployRenderAPI(RenderAPI api)
	{
		if (current == api)
			return true;

		current = api;

		API::APIBufferLayout layout = this->m_Instance->getBufferLayout();
		layout.EmployRenderAPI(api);

		uint size = this->m_Instance->getSize();
		byte* data = genew byte[size];
		this->m_Instance->getBufferData((void*)data);


		API::APIVertexBuffer* inst = API::APIVertexBuffer::Create(m_Usage);
		inst->setData(size, data);
		inst->setLayout(layout);

		if(data)
			gedel[] data;

		gedel this->m_Instance;

		if (!inst)
			return false;

		this->m_Instance = inst;

		return true;
	}

} }