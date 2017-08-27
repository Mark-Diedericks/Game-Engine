#include "ge.h"
#include "TextureCube.h"
#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Utils/ImageUtil.h"

namespace gebase { namespace graphics {

	TextureCube* TextureCube::CreateFromFile(const String& filepath)
	{
		TextureCube* thisTC = genew TextureCube();
		const byte* pixels;

		uint width;
		uint height;
		uint bits;

		pixels = LoadImage(filepath, &width, &height, &bits, true);

		thisTC->sides[0] = pixels;
		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width = width;
		thisTC->m_Height = height;

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

		thisTC->sides = const_cast<const byte**>(pixels);
		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width = width;
		thisTC->m_Height = height;

		thisTC->m_Instance = API::APITextureCube::CreateFromFiles(files[0], const_cast<const byte**>(pixels), width, height, bits);
		return thisTC;
	}

	TextureCube* TextureCube::CreateFromVerticalCross(const String* files, int32 mips)
	{
		TextureCube* thisTC = genew TextureCube();
		byte** pixels = genew byte*[mips];

		uint width;
		uint height;
		uint bits;

		for (uint i = 0; i < (uint)mips; i++)
		{
			pixels[i] = LoadImage(files[i], &width, &height, &bits, true);
		}

		thisTC->sides = const_cast<const byte**>(pixels);
		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width = width;
		thisTC->m_Height = height;

		thisTC->m_Instance = API::APITextureCube::CreateFromVerticalCross(files[0], const_cast<const byte**>(pixels), mips, width, height, bits);
		return thisTC;
	}

} }