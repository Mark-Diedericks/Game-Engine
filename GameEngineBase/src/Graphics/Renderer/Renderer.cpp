#include "ge.h"
#include "Renderer.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Init()
	{
		API::APIRenderer::Init();
		s_Instance = genew Renderer();
	}

	bool Renderer::EmployRenderAPI(RenderAPI api)
	{
		API::APIRenderer::Init();
		return true;
	}

} }