#include "ge.h"
#include "Application/Application.h"
#include "TextureCube.h"
#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"
#include "Utils/LogUtil.h"

#include "Backend/GL/GLTextureCube.h"
#include "Backend/D3D/11/DX11TextureCube.h"

namespace gebase { namespace graphics {

	std::map<TextureCube*, TextureCube*> TextureCube::s_APIChangeMap;

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
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, pixels, width, height, bits);
			//case RenderAPI::VULKAN: return genew VKTextureCube(name, pixels, width, height, bits);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, pixels, width, height, bits);
			//case RenderAPI::D3D12: return genew DX12TextureCube(name, pixels, width, height, bits);
		}

		return nullptr;
	}

	TextureCube* TextureCube::CreateFromFiles(const String& name, const byte** sides, uint width, uint height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, sides, width, height, bits);
			//case RenderAPI::VULKAN: return genew VKTextureCube(name, sides, width, height, bits);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, sides, width, height, bits);
			//case RenderAPI::D3D12: return genew DX12TextureCube(name, sides, width, height, bits);
		}

		return nullptr;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String& name, const byte** miptextures, int32 mips, uint* width, uint* height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, miptextures, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
			//case RenderAPI::VULKAN: return genew VKTextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, miptextures, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
			//case RenderAPI::D3D12: return genew DX12TextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		}

		return nullptr;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String& name, const byte*** faces, int32 mips, uint* width, uint* height, uint bits)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLTextureCube(name, faces, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
			//case RenderAPI::VULKAN: return genew VKTextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		case RenderAPI::D3D11: return genew DX11TextureCube(name, faces, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
			//case RenderAPI::D3D12: return genew DX12TextureCube(name, sides, mips, width, height, bits, InputFormat::VERTICAL_CROSS);
		}

		return nullptr;
	}

	TextureCube* TextureCube::ConvertRenderAPI(RenderAPI api, TextureCube* original)
	{
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

	void TextureCube::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<TextureCube*, TextureCube*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			//gedel ((TextureCube*)it->first);
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLTextureCube*)it->first); break;
				//case RenderAPI::VULKAN: gedel (VKTextureCube*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11TextureCube*)it->first); break;
				//case RenderAPI::D3D12: gedel ((DX12TextureCube*)it->first); break;
			}
		}

		s_APIChangeMap.clear();
	}

} }