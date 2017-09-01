#include "ge.h"
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
			std::cout << "[Texture2D] Unsupported image bit-depth: " << (int)bits << " in file: " << filepath.c_str() << std::endl;

		parameters.format = bits == 24 ? API::TextureFormat::RGB : API::TextureFormat::RGBA;

		thisT2->m_BitsPerPixel = bits;
		thisT2->m_Pixels = pixels;
		thisT2->m_Width = width;
		thisT2->m_Height = height;
		thisT2->m_Name = name;
		thisT2->m_Parameters = parameters;

		thisT2->m_Instance = API::APITexture2D::CreateFromFile(name, pixels, width, height, bits, parameters);
		return thisT2;
	}

	Texture2D* Texture2D::CreateFromFile(const String& name, const String& filepath, API::TextureLoadOptions loadOptions)
	{
		return CreateFromFile(name, filepath, API::TextureParameters(), loadOptions);
	}

	bool Texture2D::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;
		this->m_Instance = API::APITexture2D::CreateFromFile(m_Name, m_Pixels, m_Width, m_Height, m_BitsPerPixel, m_Parameters);

		if (!m_Pixels)
			this->m_Instance->setData(m_Color);

		return true;
	}

	void Texture2D::setData(const byte* pixels)
	{
		m_Pixels = (byte*)pixels;
		m_Instance->setData(pixels);
	}

	void Texture2D::setData(uint color)
	{
		m_Pixels = nullptr;
		m_Color = color;
		m_Instance->setData(color);
	}

} }