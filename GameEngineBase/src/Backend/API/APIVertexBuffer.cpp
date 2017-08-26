#include "Pch.h"
#include "Common.h"
#include "APIVertexBuffer.h"

#include "Backend/GL/GLVertexBuffer.h"
#include "Backend/D3D/11/DX11VertexBuffer.h"
//#include "Backend/D3D/12/DX12VertexBuffer.h"
//#include "Backend/VK/VKVertexBuffer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIVertexBuffer* APIVertexBuffer::Create(BufferUsage usage)
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

} } }