#include "ge.h"
#include "Common.h"
#include "APIIndexBuffer.h"

#include "Backend/GL/GLIndexBuffer.h"
#include "Backend/D3D/11/DX11IndexBuffer.h"
//#include "Backend/D3D/12/DX12IndexBuffer.h"
//#include "Backend/VK/VKIndexBuffer.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIIndexBuffer* APIIndexBuffer::Create(uint16* data, uint count)
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

	APIIndexBuffer* APIIndexBuffer::Create(uint* data, uint count)
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

} } }