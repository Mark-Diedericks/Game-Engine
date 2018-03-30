#include "ge.h"
#include "FramebufferDepth.h"
#include "Graphics/Context/Context.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLFramebufferDepth.h"
#include "Backend/D3D/11/DX11FramebufferDepth.h"

namespace gebase { namespace graphics {

	std::map<FramebufferDepth*, FramebufferDepth*> FramebufferDepth::s_APIChangeMapFramebufferDepth;
	std::vector<FramebufferDepth*> FramebufferDepth::s_CurrentFramebufferDepth;
	
	FramebufferDepth* FramebufferDepth::Create(uint width, uint height)
	{
		FramebufferDepth* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result =  genew GLFramebufferDepth(width, height); break;
		case RenderAPI::D3D11: result = genew DX11FramebufferDepth(width, height); break;
		}

		if (result != nullptr)
			s_CurrentFramebufferDepth.push_back(result);

		return result;
	}

	FramebufferDepth* FramebufferDepth::ConvertRenderAPI(RenderAPI api, FramebufferDepth* original)
	{
		if (original == nullptr)
			return nullptr;

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
		std::vector<FramebufferDepth*> temp(s_CurrentFramebufferDepth);
		s_CurrentFramebufferDepth.clear();
		s_CurrentFramebufferDepth.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void FramebufferDepth::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<FramebufferDepth*, FramebufferDepth*>::iterator it;
		for (it = s_APIChangeMapFramebufferDepth.begin(); it != s_APIChangeMapFramebufferDepth.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLFramebufferDepth*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11FramebufferDepth*)it->first); break;
			}
		}

		s_APIChangeMapFramebufferDepth.clear();
	}

} }