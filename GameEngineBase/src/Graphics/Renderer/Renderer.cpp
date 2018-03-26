#include "ge.h"
#include "Renderer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Init()
	{
		APIRenderer::Init();
		s_Instance = genew Renderer();
	}

	bool Renderer::EmployRenderAPI(RenderAPI api)
	{
		APIRenderer::Destroy();
		APIRenderer::Init();
		return true;
	}

} }