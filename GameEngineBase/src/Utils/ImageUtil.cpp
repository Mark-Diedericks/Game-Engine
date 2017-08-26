#include "Pch.h"
#include "ImageUtil.h"

#include <FreeImage.h>
#include <FreeImage/Utilities.h>

#include "System/Memory.h"
#include "System/VirtualFileSystem.h"

namespace gebase {
	
	byte* LoadImage(const char* filename, uint* width, uint* height, uint* bits, bool flipY) 
	{
		String actualPath;

		if (!VirtualFileSystem::Get()->ResolveActualPath(filename, actualPath))
			return nullptr;

		filename = actualPath.c_str();

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* fib = nullptr;

		fif = FreeImage_GetFileType(filename, 0);

		if (fif == FIF_UNKNOWN)
			if ((fif = FreeImage_GetFIFFromFilename(filename)) == FIF_UNKNOWN)
				return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			fib = FreeImage_Load(fif, filename);

		if (!fib)
		{
			std::cout << "Could not load image: " << filename << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(fib);
		FreeImage_Unload(fib);

		byte* pixels = FreeImage_GetBits(bitmap);
		uint w = FreeImage_GetWidth(bitmap);
		uint h = FreeImage_GetHeight(bitmap);
		uint b = FreeImage_GetBPP(bitmap);

		if (flipY)
			FreeImage_FlipVertical(bitmap);

		if (FreeImage_GetRedMask(bitmap) == 0xff0000)
			SwapRedBlue32(bitmap);

		if (width)
			*width = w;

		if (height)
			*height = h;

		if (bits)
			*bits = b;

		int32 size = w * h * (b / 8);
		byte* result = genew byte[size];
		
		memcpy(result, pixels, size);
		FreeImage_Unload(bitmap);
		
		return result;
	}

	byte* LoadImage(const String& filename, uint* width, uint* height, uint* bits, bool flipY) 
	{
		return LoadImage(filename.c_str(), width, height, bits, flipY);
	}

}