#include "ge.h"
#include "FramebufferDepth.h"
#include "Graphics/Context.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLFramebufferDepth.h"
#include "Backend\D3D\11\DX11FramebufferDepth.h"

namespace gebase { namespace graphics {

	std::map<FramebufferDepth*, FramebufferDepth*> FramebufferDepth::s_APIChangeMap;
	std::vector<FramebufferDepth*> FramebufferDepth::s_Current;
	
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

	void FramebufferDepth::PrepareRenderAPIChange(RenderAPI newApi)
	{
	}

	void FramebufferDepth::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<FramebufferDepth*, FramebufferDepth*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			//gedel ((FramebufferDepth*)it->first);
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLFramebufferDepth*)it->first); break;
				//case RenderAPI::VULKAN: gedel (VKFramebufferDepth*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11FramebufferDepth*)it->first); break;
				//case RenderAPI::D3D12: gedel ((DX12FramebufferDepth*)it->first); break;
			}
		}

		s_APIChangeMap.clear();
	}

} }