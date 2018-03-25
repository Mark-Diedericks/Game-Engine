#include "ge.h"
#include "Common.h"
#include "IndexBuffer.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLIndexBuffer.h"
#include "Backend\D3D\11\DX11IndexBuffer.h"

namespace gebase { namespace graphics {

	std::map<IndexBuffer*, IndexBuffer*> IndexBuffer::s_APIChangeMap;

	IndexBuffer* IndexBuffer::Create(uint16* data, uint count)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLIndexBuffer(data, count);
			//case RenderAPI::VULKAN: return genew VKIndexBuffer(data, count);
		case RenderAPI::D3D11: return genew DX11IndexBuffer(data, count);
			//case RenderAPI::D3D12: return genew DX12IndexBuffer(data, count);
		}

		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLIndexBuffer(data, count);
			//case RenderAPI::VULKAN: return genew VKIndexBuffer(data, count);
		case RenderAPI::D3D11: return genew DX11IndexBuffer(data, count);
			//case RenderAPI::D3D12: return genew DX12IndexBuffer(data, count);
		}

		return nullptr;
	}

	IndexBuffer* IndexBuffer::ConvertRenderAPI(RenderAPI api, IndexBuffer* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		IndexBuffer* buffer;

		switch (original->getLoadType())
		{
		case 0:
		{
			uint16* data16 = genew uint16[original->getCount()];
			original->getIndexData16(data16);

			buffer = Create(data16, original->getCount());

			//if (data16)
			//	gedel[] data16;
		}
			break;
		case 1:
		{
			uint* data32 = genew uint[original->getCount()];
			original->getIndexData32(data32);

			buffer = Create(data32, original->getCount());

			//if (data32)
			//	gedel[] data32;
		}
			break;
		}

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void IndexBuffer::FlushRenderAPIChange()
	{
		std::map<IndexBuffer*, IndexBuffer*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			gedel((IndexBuffer*)it->first);
		}

		s_APIChangeMap.clear();
	}

} }