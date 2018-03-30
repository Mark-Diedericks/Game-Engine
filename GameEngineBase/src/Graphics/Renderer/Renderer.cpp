#include "ge.h"
#include "Renderer.h"

#include "Graphics/Context/Context.h"
#include "System/Memory.h"

#include "Backend/GL/GLRenderer.h"
#include "Backend/D3D/11/DX11Renderer.h"

namespace gebase { namespace graphics {

	Renderer* Renderer::s_Instance = nullptr;
	Renderer* Renderer::s_PreviousInstance = nullptr;

	void Renderer::Init()
	{
		if (s_Instance != nullptr)
			s_PreviousInstance = s_Instance;

		s_Instance = nullptr;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: s_Instance = genew GLRenderer(); break;
			//case RenderAPI::VULKAN: s_Instance = genew VKRenderer(); break;
		case RenderAPI::D3D11: s_Instance = genew DX11Renderer(); break;
			//case RenderAPI::D3D12: s_Instance = genew DX12Renderer(); break;
		}

		s_Instance->InitInternal();
	}

	bool Renderer::EmployRenderAPI(RenderAPI api)
	{
		Renderer::Init();
		return true;
	}

} }