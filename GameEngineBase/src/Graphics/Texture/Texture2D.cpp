#include "ge.h"
#include "Texture2D.h"
#include "Application/Application.h"

#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"
#include "Utils\LogUtil.h"

#include "Backend/GL/GLTexture2D.h"
#include "Backend/D3D/11/DX11Texture2D.h"

namespace gebase { namespace graphics {

	std::map<Texture2D*, Texture2D*> Texture2D::s_APIChangeMap;
	
	Texture2D* Texture2D::CreateFromFile(const String& filepath, TextureParameters parameters, TextureLoadOptions loadOptions)
	{
		return CreateFromFile(filepath, filepath, parameters, loadOptions);
	}

	Texture2D* Texture2D::CreateFromFile(const String& filepath, TextureLoadOptions loadOptions)
	{
		return CreateFromFile(filepath, filepath, TextureParameters(), loadOptions);
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, TextureParameters parameters, TextureLoadOptions loadOptions)
	{
		uint bits;
		uint width;
		uint height;
		byte* pixels = GELoadImage(filepath.c_str(), &width, &height, &bits, !loadOptions.flipY); //FreeImage loads from the bottom up to the top

		if (bits != 24 && bits != 32)
			utils::LogUtil::WriteLine("ERROR", "[Texture2D] Unsupported image bit-depth: " + std::to_string(bits) + " in file: " + filepath);

		parameters.format = bits == 24 ? TextureFormat::RGB : TextureFormat::RGBA;

		return CreateFromFile(name, pixels, width, height, bits, parameters);
	}	
	
	Texture2D* Texture2D::Create(uint width, uint height, TextureParameters parameters)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTexture2D(width, height, parameters);
			//case RenderVULKAN: return genew VKTexture2D(width, height, parameters);
		case RenderAPI::D3D11: return genew DX11Texture2D(width, height, parameters);
			//case RenderD3D12: return genew DX12Texture2D(width, height, parameters);
		}

		return nullptr;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTexture2D(name, pixels, width, height, bits, parameters);
			//case RenderVULKAN: return genew VKTexture2D(name, pixels, width, height, bits, parameters);
		case RenderAPI::D3D11: return genew DX11Texture2D(name, pixels, width, height, bits, parameters);
			//case RenderD3D12: return genew DX12Texture2D(name, pixels, width, height, bits, parameters);
		}

		return nullptr;
	}

	Texture2D* Texture2D::ConvertRenderAPI(RenderAPI api, Texture2D* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		byte* data = genew byte[original->getSize()];
		original->getPixelData(data);

		Texture2D* texture;

		switch (original->getLoadType())
		{
		case 0:
		{
			texture = Create(original->getWidth(), original->getHeight(), original->getParameters());
			texture->setData(data);

			if (data)
				gedel[] data;
		}
			break;
		case 1:
		{
			texture = CreateFromFile(original->getName(), data, original->getWidth(), original->getHeight(), original->getBitsPerPixel(), original->getParameters());
		}
			break;
		}

		//if (data)
		//	gedel[] data;

		texture->setResourceName(original->getResourceName());
		AddRenderAPIChange(original, texture);

		return texture;
	}

	void Texture2D::FlushRenderAPIChange()
	{
		std::map<Texture2D*, Texture2D*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			gedel((Texture2D*)it->first);
		}

		s_APIChangeMap.clear();
	}

} }