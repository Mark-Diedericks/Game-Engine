#include "ge.h"
#include "Common.h"
#include "VertexBuffer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLVertexBuffer.h"
#include "Backend/D3D/11/DX11VertexBuffer.h"

namespace gebase { namespace graphics {

	std::map<VertexBuffer*, VertexBuffer*> VertexBuffer::s_APIChangeMap;
	std::vector<VertexBuffer*> VertexBuffer::s_Current;

	VertexBuffer* VertexBuffer::Create(BufferUsage usage)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLVertexBuffer(usage);
			//case RenderAPI::VULKAN: return genew VKVertexBuffer(usage);
		case RenderAPI::D3D11: return genew DX11VertexBuffer(usage);
			//case RenderAPI::D3D12: return genew DX12VertexBuffer(usage);
		}

		return nullptr;
	}

	VertexBuffer* VertexBuffer::ConvertRenderAPI(RenderAPI api, VertexBuffer* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		BufferLayout layout = original->getBufferLayout();
		layout.EmployRenderAPI(api);

		uint size = original->getSize();
		byte* data = genew byte[size];
		original->getBufferData(data);

		VertexBuffer* buffer = Create(original->m_Usage);
		buffer->setData(size, data);
		buffer->setLayout(layout);

		//if (data)
		//	gedel[] data;

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void VertexBuffer::PrepareRenderAPIChange(RenderAPI newApi)
	{
	}

	void VertexBuffer::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<VertexBuffer*, VertexBuffer*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			//gedel ((VertexBuffer*)it->first);
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLVertexBuffer*)it->first); break;
				//case RenderAPI::VULKAN: gedel (VKTextureDepth*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11VertexBuffer*)it->first); break;
				//case RenderAPI::D3D12: gedel ((DX12TextureDepth*)it->first); break;
			}
		}

		s_APIChangeMap.clear();
	}

} }