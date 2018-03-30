#include "ge.h"
#include "TextureDepth.h"
#include "Graphics/Context/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLTextureDepth.h"
#include "Backend/D3D/11/DX11TextureDepth.h"

namespace gebase { namespace graphics {

	std::map<TextureDepth*, TextureDepth*> TextureDepth::s_APIChangeMapTextureDepth;
	std::vector<TextureDepth*> TextureDepth::s_CurrentTextureDepth;

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		TextureDepth* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTextureDepth(width, height); break;
		case RenderAPI::D3D11: result = genew DX11TextureDepth(width, height); break;
		}

		if (result != nullptr)
			s_CurrentTextureDepth.push_back(result);

		return result;
	}

	TextureDepth* TextureDepth::ConvertRenderAPI(RenderAPI api, TextureDepth* original)
	{
		if (original == nullptr)
			return nullptr;

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

	void TextureDepth::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<TextureDepth*> temp(s_CurrentTextureDepth);
		s_CurrentTextureDepth.clear();
		s_CurrentTextureDepth.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void TextureDepth::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<TextureDepth*, TextureDepth*>::iterator it;
		for (it = s_APIChangeMapTextureDepth.begin(); it != s_APIChangeMapTextureDepth.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel ((GLTextureDepth*)it->first); break;
			case RenderAPI::D3D11: gedel ((DX11TextureDepth*)it->first); break;
			}
		}

		s_APIChangeMapTextureDepth.clear();
	}

} }