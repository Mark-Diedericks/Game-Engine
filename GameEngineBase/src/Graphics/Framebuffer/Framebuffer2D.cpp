#include "ge.h"
#include "Framebuffer2D.h"
#include "Graphics/Context.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLFramebuffer2D.h"
#include "Backend\D3D\11\DX11Framebuffer2D.h"

namespace gebase { namespace graphics {

	std::map<Framebuffer2D*, Framebuffer2D*> Framebuffer2D::s_APIChangeMap;
	std::vector<Framebuffer2D*> Framebuffer2D::s_Current;

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLFramebuffer2D(width, height);
			//case RenderAPI::VULKAN: return genew VKFramebuffer2D(width, height);
		case RenderAPI::D3D11: return genew DX11Framebuffer2D(width, height);
			//case RenderAPI::D3D12: return genew DX12Framebuffer2D(width, height);
		}

		return nullptr;
	}

	Framebuffer2D* Framebuffer2D::ConvertRenderAPI(RenderAPI api, Framebuffer2D* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		Framebuffer2D* framebuffer = Create(original->getWidth(), original->getHeight());

		AddRenderAPIChange(original, framebuffer);

		return framebuffer;
	}

	void Framebuffer2D::PrepareRenderAPIChange(RenderAPI newApi)
	{
	}

	void Framebuffer2D::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<Framebuffer2D*, Framebuffer2D*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			//gedel ((Framebuffer2D*)it->first);
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLFramebuffer2D*)it->first); break;
				//case RenderAPI::VULKAN: gedel (VKFramebuffer2D*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11Framebuffer2D*)it->first); break;
				//case RenderAPI::D3D12: gedel ((DX12Framebuffer2D*)it->first); break;
			}
		}

		s_APIChangeMap.clear();
	}

} }