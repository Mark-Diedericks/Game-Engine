#include "Pch.h"
#include "Texture2D.h"

#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"

namespace gebase { namespace graphics {

	Texture2D* Texture2D::Create(uint width, uint height, API::TextureParameters parameters, API::TextureLoadOptions loadOptions)
	{
		Texture2D* thisT2 = genew Texture2D();
		thisT2->m_BitsPerPixel = parameters.format == API::TextureFormat::RGB ? 3 : 4;
		thisT2->m_Instance = API::APITexture2D::Create(width, height, parameters);
		return thisT2;
	}

	Texture2D* Texture2D::CreateFromFile(const String& filepath, API::TextureParameters parameters, API::TextureLoadOptions loadOptions)
	{
		return CreateFromFile(filepath, filepath, parameters, loadOptions);
	}

	Texture2D* Texture2D::CreateFromFile(const String& filepath, API::TextureLoadOptions loadOptions)
	{
		return CreateFromFile(filepath, API::TextureParameters(), loadOptions);
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, API::TextureParameters parameters, API::TextureLoadOptions loadOptions)
	{
		Texture2D* thisT2 = genew Texture2D();
		thisT2->m_BitsPerPixel = parameters.format == API::TextureFormat::RGB ? 3 : 4;

		uint bits;
		uint width;
		uint height;
		byte* pixels = LoadImage(filepath.c_str(), &width, &height, &bits, !loadOptions.flipY); //FreeImage loads from the bottom up to the top

		if (bits != 24 && bits != 32)
			std::cout << "[Texture2D] Unsupported image bit-depth: " << StringFormat::Float(bits, 0).c_str() << std::endl;

		parameters.format = bits == 24 ? API::TextureFormat::RGB : API::TextureFormat::RGBA;

		thisT2->m_BitsPerPixel = bits;
		thisT2->m_Pixels = pixels;

		thisT2->m_Instance = API::APITexture2D::CreateFromFile(name, pixels, width, height, bits, parameters);
		return thisT2;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, API::TextureLoadOptions loadOptions)
	{
		return CreateFromFile(name, filepath, API::TextureParameters(), loadOptions);
	}

	void Texture2D::setData(const byte* pixels)
	{
		m_Color = NULL;
		m_Pixels = pixels;
		m_Instance->setData(pixels);
	}

	void Texture2D::setData(uint color)
	{
		m_Pixels = nullptr;
		m_Color = color;
		m_Instance->setData(color);
	}

} }