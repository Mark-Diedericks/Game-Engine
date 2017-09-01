#include "ge.h"
#include "APIBufferLayout.h"

namespace gebase { namespace graphics { namespace API {

	APIBufferLayout::APIBufferLayout() : m_Size(0) 
	{
		m_RenderAPI = graphics::Context::getRenderAPI();
	}

	void APIBufferLayout::Push(const String& name, uint type, uint size, uint count, bool normalized)
	{
		m_Layout.push_back({ name, type, size, count, m_Size, normalized });
		m_Size += size * count;
	}

	void APIBufferLayout::EmployRenderAPI(RenderAPI api)
	{
		switch (m_RenderAPI)
		{
		case gebase::graphics::RenderAPI::OPENGL:
			m_Layout = GLConvertTo(api);
			break;
		case gebase::graphics::RenderAPI::D3D11:
			m_Layout = DX11ConvertTo(api);
			break;
		}
	}

	std::vector<BufferElement> APIBufferLayout::GLConvertTo(RenderAPI api)
	{
		std::vector<BufferElement> layout;

		switch (api)
		{
		case gebase::graphics::RenderAPI::OPENGL:
			
			break;
		case gebase::graphics::RenderAPI::D3D11:
			
			break;
		}

		return layout;
	}

	std::vector<BufferElement> APIBufferLayout::DX11ConvertTo(RenderAPI api)
	{
		std::vector<BufferElement> layout;

		switch (api)
		{
		case gebase::graphics::RenderAPI::OPENGL:
			
			break;
		case gebase::graphics::RenderAPI::D3D11:
			
			break;
		}

		return layout;
	}

} } }