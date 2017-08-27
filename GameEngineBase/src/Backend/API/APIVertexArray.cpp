#include "ge.h"
#include "Common.h"
#include "APIVertexArray.h"

#include "Backend/GL/GLVertexArray.h"
#include "Backend/D3D/11/DX11VertexArray.h"
//#include "Backend/D3D/12/DX12VertexArray.h"
//#include "Backend/VK/VKVertexArray.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIVertexArray* APIVertexArray::Create()
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

} } }