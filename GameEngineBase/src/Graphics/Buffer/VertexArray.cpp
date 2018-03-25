#include "ge.h"
#include "Common.h"
#include "VertexArray.h"

#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLVertexArray.h"
#include "Backend\D3D\11\DX11VertexArray.h"

namespace gebase { namespace graphics {

	std::map<VertexArray*, VertexArray*> VertexArray::s_APIChangeMap;

	VertexArray* VertexArray::Create()
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLVertexArray();
			//case RenderAPI::VULKAN: return genew VKVertexArray();
		case RenderAPI::D3D11: return genew DX11VertexArray();
			//case RenderAPI::D3D12: return genew DX12VertexArray();
		}

		return nullptr;
	}

	VertexArray* VertexArray::ConvertRenderAPI(RenderAPI api, VertexArray* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		std::vector<VertexBuffer*> buffers(original->getBuffers());

		for (uint i = 0; i < buffers.size(); i++)
			buffers[i] = VertexBuffer::ConvertRenderAPI(api, buffers[i]);

		VertexArray* buffer = Create();
		
		for (uint i = 0; i < buffers.size(); i++)
			buffer->PushBuffer(buffers[i]);

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void VertexArray::FlushRenderAPIChange()
	{
		std::map<VertexArray*, VertexArray*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			gedel((VertexArray*)it->first);
		}

		s_APIChangeMap.clear();
	}

} }