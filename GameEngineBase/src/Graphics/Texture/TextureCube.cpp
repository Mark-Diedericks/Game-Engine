#include "ge.h"
#include "Application/Application.h"
#include "TextureCube.h"
#include "Graphics/Context/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"
#include "Utils/LogUtil.h"

#include "Backend/GL/GLTextureCube.h"
#include "Backend/D3D/11/DX11TextureCube.h"

namespace gebase { namespace graphics {

	std::map<TextureCube*, TextureCube*> TextureCube::s_APIChangeMapTextureCube;
	std::vector<TextureCube*> TextureCube::s_CurrentTextureCube;

	TextureCube* TextureCube::CreateFromFile(const String& filepath)
	{
		byte* pixels;

		uint width;
		uint height;
		uint bits;

		pixels = GELoadImage(filepath, &width, &height, &bits, true);

		return TextureCube::CreateFromFile(filepath, pixels, width, height, bits);
	}

	TextureCube* TextureCube::CreateFromFiles(const String* files)
	{
		byte** pixels = genew byte*[6];

		uint width;
		uint height;
		uint bits;

		pixels[0] = GELoadImage(files[0], &width, &height, &bits, true);
		pixels[1] = GELoadImage(files[1], &width, &height, &bits, true);
		pixels[2] = GELoadImage(files[2], &width, &height, &bits, true);
		pixels[3] = GELoadImage(files[3], &width, &height, &bits, true);
		pixels[4] = GELoadImage(files[4], &width, &height, &bits, true);
		pixels[5] = GELoadImage(files[5], &width, &height, &bits, true);

		return TextureCube::CreateFromFiles(files[0], const_cast<const byte**>(pixels), width, height, bits);
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String* files, int32 mips)
	{
		byte** pixels = genew byte*[mips];

		uint* width = genew uint[mips];
		uint* height = genew uint[mips];
		uint bits = 32;

		for (uint i = 0; i < (uint)mips; i++)
		{
			pixels[i] = GELoadImage(files[i], &width[i], &height[i], &bits, true);
		}

		return TextureCube::CreateFromVerticalCross(files[0], const_cast<const byte**>(pixels), mips, width, height, bits);
	}

	TextureCube* TextureCube::CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits)
	{
		TextureCube* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTextureCube(name, pixels, width, height, bits); break;
		case RenderAPI::D3D11: result = genew DX11TextureCube(name, pixels, width, height, bits); break;
		}

		if (result != nullptr)
			s_CurrentTextureCube.push_back(result);

		return result;
	}

	TextureCube* TextureCube::CreateFromFiles(const String& name, const byte** sides, uint width, uint height, uint bits)
	{
		TextureCube* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTextureCube(name, sides, width, height, bits); break;
		case RenderAPI::D3D11: result = genew DX11TextureCube(name, sides, width, height, bits); break;
		}

		if (result != nullptr)
			s_CurrentTextureCube.push_back(result);

		return result;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String& name, const byte** miptextures, int32 mips, uint* width, uint* height, uint bits)
	{
		TextureCube* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTextureCube(name, miptextures, mips, width, height, bits, InputFormat::VERTICAL_CROSS); break;
		case RenderAPI::D3D11: result = genew DX11TextureCube(name, miptextures, mips, width, height, bits, InputFormat::VERTICAL_CROSS); break;
		}

		if (result != nullptr)
			s_CurrentTextureCube.push_back(result);

		return result;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String& name, const byte*** faces, int32 mips, uint* width, uint* height, uint bits)
	{
		TextureCube* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLTextureCube(name, faces, mips, width, height, bits, InputFormat::VERTICAL_CROSS); break;
		case RenderAPI::D3D11: result = genew DX11TextureCube(name, faces, mips, width, height, bits, InputFormat::VERTICAL_CROSS); break;
		}

		if (result != nullptr)
			s_CurrentTextureCube.push_back(result);

		return result;
	}

	TextureCube* TextureCube::ConvertRenderAPI(RenderAPI api, TextureCube* original)
	{
		if (original == nullptr)
			return nullptr;

		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		TextureCube* texture;

		switch (original->getLoadType())
		{
		case 0:
		{
			utils::LogUtil::WriteLine("ERROR", "[TextureCube] EmployRenderAPI() - Method 0 is not implemented.");
		}
			break;
		case 1:
		{
			utils::LogUtil::WriteLine("ERROR", "[TextureCube] EmployRenderAPI() - Method 1 is not implemented.");
		}
			break;
		case 2:
		{
			uint* fWidth = genew uint[original->getMips()];
			for (uint i = 0; i < original->getMips(); i++)
				fWidth[i] = original->getWidth(i) / 3;

			uint* fHeight = genew uint[original->getMips()];
			for (uint i = 0; i < original->getMips(); i++)
				fHeight[i] = original->getHeight(i) / 4;

			byte*** faces = genew byte**[original->getMips()];
			uint stride = original->getBitsPerPixel() / 8;

			for (uint i = 0; i < original->getMips(); i++)
			{
				faces[i] = genew byte*[6];
				for (uint f = 0; f < 6; f++)
				{
					faces[i][f] = genew byte[fWidth[i] * fHeight[i] * stride];
				}
			}

			original->getPixelData(faces);
			texture = CreateFromVerticalCross(original->getName(), const_cast<const byte***>(faces), original->getMips(), fWidth, fHeight, original->getBitsPerPixel());

			/*for (uint i = 0; i < original->getMips(); i++)
			{
				for (uint f = 0; f < 6; f++)
				{
					gedel[] faces[i][f];
				}
				gedel[] faces[i];
			}
			gedel[] faces;*/
		}
			break;
		}

		texture->setResourceName(original->getResourceName());
		AddRenderAPIChange(original, texture);

		return texture;
	}

	void TextureCube::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<TextureCube*> temp(s_CurrentTextureCube);
		s_CurrentTextureCube.clear();
		s_CurrentTextureCube.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void TextureCube::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<TextureCube*, TextureCube*>::iterator it;
		for (it = s_APIChangeMapTextureCube.begin(); it != s_APIChangeMapTextureCube.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLTextureCube*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11TextureCube*)it->first); break;
			}
		}

		s_APIChangeMapTextureCube.clear();
	}

} }