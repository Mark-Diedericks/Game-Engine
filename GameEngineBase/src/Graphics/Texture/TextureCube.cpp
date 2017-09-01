#include "ge.h"
#include "TextureCube.h"
#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"

namespace gebase { namespace graphics {

	TextureCube* TextureCube::CreateFromFile(const String& filepath)
	{
		TextureCube* thisTC = genew TextureCube();
		byte* pixels;

		uint width;
		uint height;
		uint bits;

		pixels = LoadImage(filepath, &width, &height, &bits, true);

		thisTC->sides[0] = pixels;
		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width[0] = width;
		thisTC->m_Height[0] = height;
		thisTC->m_Name = filepath;
		thisTC->m_LoadType = 0;

		thisTC->m_Instance = API::APITextureCube::CreateFromFile(filepath, pixels, width, height, bits);

		return thisTC;
	}

	TextureCube* TextureCube::CreateFromFiles(const String* files)
	{
		TextureCube* thisTC = genew TextureCube();
		byte** pixels = genew byte*[6];

		uint width;
		uint height;
		uint bits;

		pixels[0] = LoadImage(files[0], &width, &height, &bits, true);
		pixels[1] = LoadImage(files[1], &width, &height, &bits, true);
		pixels[2] = LoadImage(files[2], &width, &height, &bits, true);
		pixels[3] = LoadImage(files[3], &width, &height, &bits, true);
		pixels[4] = LoadImage(files[4], &width, &height, &bits, true);
		pixels[5] = LoadImage(files[5], &width, &height, &bits, true);

		thisTC->sides = pixels;
		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width[0] = width;
		thisTC->m_Height[0] = height;
		thisTC->m_Name = files[0];
		thisTC->m_LoadType = 1;

		thisTC->m_Instance = API::APITextureCube::CreateFromFiles(files[0], const_cast<const byte**>(pixels), width, height, bits);
		return thisTC;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String* files, int32 mips)
	{
		TextureCube* thisTC = genew TextureCube();
		byte** pixels = genew byte*[mips];

		uint* width = genew uint[mips];
		uint* height = genew uint[mips];
		uint* bits = genew uint[mips];

		for (uint i = 0; i < (uint)mips; i++)
		{
			pixels[i] = LoadImage(files[i], &width[i], &height[i], &bits[i], true);
		}

		thisTC->sides = pixels;
		thisTC->m_BitsPerPixel = bits[0];
		thisTC->m_Width = width;
		thisTC->m_Height = height;
		thisTC->m_Mips = mips;
		thisTC->m_Name = files[0];
		thisTC->m_LoadType = 2;

		thisTC->m_Instance = API::APITextureCube::CreateFromVerticalCross(files[0], const_cast<const byte**>(pixels), mips, width, height, bits[0]);
		return thisTC;
	}

	bool TextureCube::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;
		switch (m_LoadType)
		{
		case 0:
			this->m_Instance = API::APITextureCube::CreateFromFile(m_Name, sides[0], m_Width[0], m_Height[0], m_BitsPerPixel);
			break;
		case 1:
			this->m_Instance = API::APITextureCube::CreateFromFiles(m_Name, const_cast<const byte**>(sides), m_Width[0], m_Height[0], m_BitsPerPixel);
			break;
		case 2:
			this->m_Instance = API::APITextureCube::CreateFromVerticalCross(m_Name, const_cast<const byte**>(sides), m_Mips, m_Width, m_Height, m_BitsPerPixel);
			break;
		}

		return true;
	}

} }