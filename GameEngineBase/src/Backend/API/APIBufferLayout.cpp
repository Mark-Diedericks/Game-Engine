#include "ge.h"
#include "APIBufferLayout.h"
#include "APIConvert.h"

namespace gebase { namespace graphics { namespace API {

	APIBufferLayout::APIBufferLayout() : m_Size(0) 
	{
		m_RenderAPI = graphics::Context::getRenderAPI();
	}

	void APIBufferLayout::Push(const String& name, BufferElementType type, BufferElementCount count, bool normalized)
	{
		m_Layout.push_back({ name, type, count, m_Size, normalized });
		m_Size += APIConvert::BufferElementSize(type) * count.getCurrentCount();
	}

	void APIBufferLayout::EmployRenderAPI(RenderAPI api)
	{
		m_Size = 0;
		m_RenderAPI = api;
		
		for (uint i = 0; i < m_Layout.size(); i++)
		{
			m_Layout[i].offset = m_Size;
			m_Size += APIConvert::BufferElementSize(m_Layout[i].type, api) * m_Layout[i].count.getCurrentCount();
		}
	}

} } }