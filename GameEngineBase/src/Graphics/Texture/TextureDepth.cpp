#include "ge.h"
#include "TextureDepth.h"
#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLTextureDepth.h"
#include "Backend/D3D/11/DX11TextureDepth.h"

namespace gebase { namespace graphics {

	std::map<TextureDepth*, TextureDepth*> TextureDepth::s_APIChangeMap;

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureDepth(width, height);
			//case RenderAPI::VULKAN: return genew VKTextureDepth(width, height);
		case RenderAPI::D3D11: return genew DX11TextureDepth(width, height);
			//case RenderAPI::D3D12: return genew DX12TextureDepth(width, height);
		}

		return nullptr;
	}

	TextureDepth* TextureDepth::ConvertRenderAPI(RenderAPI api, TextureDepth* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		uint16* data = genew uint16[original->getSize()];
		original->getPixelData(data);

		TextureDepth* texture = Create(original->getWidth(), original->getHeight());
		texture->setData(data);

		//if (data)
		//	gedel[] data;

		texture->setResourceName(original->getResourceName());
		AddRenderAPIChange(original, texture);

		return texture;
	}

	void TextureDepth::FlushRenderAPIChange()
	{
		std::map<TextureDepth*, TextureDepth*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			gedel((TextureDepth*)it->first);
		}

		s_APIChangeMap.clear();
	}

} }