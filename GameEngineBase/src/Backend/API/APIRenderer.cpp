#include "Pch.h"
#include "APIRenderer.h"

#include "Backend/GL/GLRenderer.h"
#include "Backend/D3D/11/DX11Renderer.h"
//#include "Backend/D3D/12/DX12Renderer.h"
//#include "Backend/VK/VKRenderer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	APIRenderer* APIRenderer::s_Instance = nullptr;

	void APIRenderer::Init()
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: s_Instance = genew GLRenderer(); break;
		//case RenderAPI::VULKAN: s_Instance = genew VKRenderer(); break;
		case RenderAPI::D3D11: s_Instance = genew DX11Renderer(); break;
		//case RenderAPI::D3D12: s_Instance = genew DX12Renderer(); break;
		}

		s_Instance->InitInternal();
	}

} } }