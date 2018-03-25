#include "ge.h"
#include "BufferLayout.h"
#include "Backend\API\APIConvert.h"

namespace gebase { namespace graphics {

	BufferLayout::BufferLayout() : m_Size(0) 
	{
		m_RenderAPI = graphics::Context::getRenderAPI();
	}

	void BufferLayout::Push(const String& name, BufferElementType type, BufferElementCount count, bool normalized)
	{
		m_Layout.push_back({ name, type, count, m_Size, normalized });
		m_Size += APIConvert::GetBufferElementSize(type) * count.getCurrentCount();
	}

	void BufferLayout::EmployRenderAPI(RenderAPI api)
	{
		if (m_RenderAPI == api)
			return;

		m_Size = 0;
		m_RenderAPI = api;
		
		for (uint i = 0; i < m_Layout.size(); i++)
		{
			m_Layout[i].offset = m_Size;
			m_Size += APIConvert::GetBufferElementSize(m_Layout[i].type, api) * m_Layout[i].count.getCurrentCount();
		}
	}

} }