#include "ge.h"
#include "FramebufferDepth.h"
#include "Graphics/Context.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLFramebufferDepth.h"
#include "Backend\D3D\11\DX11FramebufferDepth.h"

namespace gebase { namespace graphics {

	std::map<FramebufferDepth*, FramebufferDepth*> FramebufferDepth::s_APIChangeMap;
	
	FramebufferDepth* FramebufferDepth::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLFramebufferDepth(width, height);
			//case RenderAPI::VULKAN: return genew VKFramebufferDepth(width, height);
		case RenderAPI::D3D11: return genew DX11FramebufferDepth(width, height);
			//case RenderAPI::D3D12: return genew DX12FramebufferDepth(width, height);
		}

		return nullptr;
	}

	FramebufferDepth* FramebufferDepth::ConvertRenderAPI(RenderAPI api, FramebufferDepth* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		FramebufferDepth* framebuffer = Create(original->getWidth(), original->getHeight());

		AddRenderAPIChange(original, framebuffer);

		return framebuffer;
	}

	void FramebufferDepth::FlushRenderAPIChange()
	{
		std::map<FramebufferDepth*, FramebufferDepth*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			gedel((FramebufferDepth*)it->first);
		}

		s_APIChangeMap.clear();
	}

} }