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
		API::APIBufferLayout layout = m_Instance->getBufferLayout();
		layout.EmployRenderAPI(api);

		void* data = m_Instance->getBufferData();
		uint size = m_Instance->getSize();

		gedel m_Instance;

		m_Instance = API::APIVertexBuffer::Create(m_Usage);
		setData(size, data);
		setLayout(layout);

		return true;
	}

} }