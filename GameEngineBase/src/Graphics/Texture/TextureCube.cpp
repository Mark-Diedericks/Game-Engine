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
		uint bits = 32;

		for (uint i = 0; i < (uint)mips; i++)
		{
			pixels[i] = LoadImage(files[i], &width[i], &height[i], &bits, true);
		}

		thisTC->m_BitsPerPixel = bits;
		thisTC->m_Width = width;
		thisTC->m_Height = height;
		thisTC->m_Mips = mips;
		thisTC->m_Name = files[0];
		thisTC->m_LoadType = 2;

		thisTC->m_Instance = API::APITextureCube::CreateFromVerticalCross(files[0], const_cast<const byte**>(pixels), mips, width, height, bits);
		return thisTC;
	}

	bool TextureCube::EmployRenderAPI(RenderAPI api)
	{
		if (current == api)
			return true;

		current = api;
		
		API::APITextureCube* inst;

		switch (m_LoadType)
		{
		case 0:
			std::cout << "[TextureCube] EmployRenderAPI() - Method 1 is not implemented." << std::endl;
			break;
		case 1:
			std::cout << "[TextureCube] EmployRenderAPI() - Method 2 is not implemented." << std::endl;
			break;
		case 2:
			uint* fWidth = genew uint[m_Mips];
			for (uint i = 0; i < m_Mips; i++)
				fWidth[i] = m_Width[i] / 3;

			uint* fHeight = genew uint[m_Mips];
			for (uint i = 0; i < m_Mips; i++)
				fHeight[i] = m_Height[i] / 4;		
			
			byte*** faces = genew byte**[m_Mips];
			uint stride = m_BitsPerPixel / 8;

			for (uint i = 0; i < m_Mips; i++)
			{
				faces[i] = genew byte*[6];
				for (int f = 0; f < 6; f++)
				{
					faces[i][f] = genew byte[fWidth[i] * fHeight[i] * stride];
				}
			}

			this->m_Instance->getPixelData(faces);

			inst = API::APITextureCube::CreateFromVerticalCross(m_Name, const_cast<const byte***>(faces), m_Mips, fWidth, fHeight, m_BitsPerPixel);
			break;
		}

		gedel this->m_Instance;

		if (!inst)
			return false;

		this->m_Instance = inst;

		return true;
	}

} }