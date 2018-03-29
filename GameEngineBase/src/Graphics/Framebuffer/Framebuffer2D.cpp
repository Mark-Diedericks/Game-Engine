#include "ge.h"
#include "Framebuffer2D.h"
#include "Graphics/Context.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLFramebuffer2D.h"
#include "Backend\D3D\11\DX11Framebuffer2D.h"

namespace gebase { namespace graphics {

	std::map<Framebuffer2D*, Framebuffer2D*> Framebuffer2D::s_APIChangeMapFramebuffer2D;
	std::vector<Framebuffer2D*> Framebuffer2D::s_CurrentFramebuffer2D;

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		Framebuffer2D* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLFramebuffer2D(width, height); break;
		case RenderAPI::D3D11: result = genew DX11Framebuffer2D(width, height); break;
		}

		if (result != nullptr)
			s_CurrentFramebuffer2D.push_back(result);

		return result;
	}

	Framebuffer2D* Framebuffer2D::ConvertRenderAPI(RenderAPI api, Framebuffer2D* original)
	{
		if (original == nullptr)
			return nullptr;

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
		std::vector<Framebuffer2D*> temp(s_CurrentFramebuffer2D);
		s_CurrentFramebuffer2D.clear();
		s_CurrentFramebuffer2D.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void Framebuffer2D::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<Framebuffer2D*, Framebuffer2D*>::iterator it;
		for (it = s_APIChangeMapFramebuffer2D.begin(); it != s_APIChangeMapFramebuffer2D.end(); it++)
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

		s_APIChangeMapFramebuffer2D.clear();
	}

} }