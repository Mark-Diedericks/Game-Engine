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

	std::map<Texture2D*, Texture2D*> Texture2D::s_APIChangeMapTexture2D;
	std::vector<Texture2D*> Texture2D::s_CurrentTexture2D;
	
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
		Texture2D* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTexture2D(width, height, parameters); break;
		case RenderAPI::D3D11: result = genew DX11Texture2D(width, height, parameters); break;
		}

		if (result != nullptr)
			s_CurrentTexture2D.push_back(result);

		return result;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters)
	{
		Texture2D* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTexture2D(name, pixels, width, height, bits, parameters); break;
		case RenderAPI::D3D11: result = genew DX11Texture2D(name, pixels, width, height, bits, parameters); break;
		}

		if (result != nullptr)
			s_CurrentTexture2D.push_back(result);

		return result;
	}

	Texture2D* Texture2D::ConvertRenderAPI(RenderAPI api, Texture2D* original)
	{
		if (original == nullptr)
			return nullptr;

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

			//if (data)
			//	gedel[] data;
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

	void Texture2D::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<Texture2D*> temp(s_CurrentTexture2D);
		s_CurrentTexture2D.clear();
		s_CurrentTexture2D.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void Texture2D::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<Texture2D*, Texture2D*>::iterator it;
		for (it = s_APIChangeMapTexture2D.begin(); it != s_APIChangeMapTexture2D.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLTexture2D*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11Texture2D*)it->first); break;
			}
		}

		s_APIChangeMapTexture2D.clear();
	}

} }