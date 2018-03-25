#include "ge.h"
#include "Common.h"
#include "DX11VertexArray.h"

#include "DX11Common.h"
#include "DX11Context.h"
#include "DX11VertexBuffer.h"

namespace gebase { namespace graphics {

	DX11VertexArray::DX11VertexArray() : VertexArray(0)
	{

	}

	DX11VertexArray::~DX11VertexArray()
	{
		
	}

	void DX11VertexArray::PushBuffer(gebase::graphics::VertexBuffer* buffer)
	{
		m_Buffers.push_back(buffer);
	}

	void DX11VertexArray::Bind() const
	{

	}

	void DX11VertexArray::Unbind() const
	{

	}

	void DX11VertexArray::Draw(uint count, uint index) const
	{
		
	}

	void DX11VertexArray::Draw(uint count) const
	{
		for (uint i = 0; i < m_Buffers.size(); i++)
		{
			m_Buffers[i]->Bind();
			DX11Context::getDeviceContext()->DrawIndexed(count, 0, 0);
			m_Buffers[i]->Unbind();
		}
	}

} }