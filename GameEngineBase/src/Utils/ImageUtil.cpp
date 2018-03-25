#include "ge.h"
#include "ImageUtil.h"

#include <FreeImage.h>
#include <FreeImage/Utilities.h>

#include "System/Memory.h"
#include "System/VirtualFileSystem.h"

#include "Utils\LogUtil.h"

namespace gebase {
	
	byte* GELoadImage(const char* filename, uint* width, uint* height, uint* bits, bool flipY) 
	{
		String actualPath;

		if (!VirtualFileSystem::Get()->ResolveActualPath(filename, actualPath))
		{
			utils::LogUtil::WriteLine("ERROR", "[ImageUtil] GELoadImage() - Could not resolve the actual path for the file; " + (String)filename);
			return nullptr;
		}

		filename = actualPath.c_str();

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* fib = nullptr;

		fif = FreeImage_GetFileType(filename, 0);

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);

		if (fif == FIF_UNKNOWN)
		{
			utils::LogUtil::WriteLine("ERROR", "[ImageUtil] GELoadImage() - Could not find a compatiable fif in the file: " + (String)filename);
			return nullptr;
		}

		if (FreeImage_FIFSupportsReading(fif))
			fib = FreeImage_Load(fif, filename);

		if (!fib)
		{
			utils::LogUtil::WriteLine("ERROR", "[ImageUtil] GELoadImage() - Could not load image: " + (String)filename);
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
		
		utils::LogUtil::WriteLine("INFO", "[ImageUtil] GELoadImage() - the width is; " + std::to_string(w) + " the height is; " + std::to_string(h) + " the bit depth is; " + std::to_string(b));

		memcpy(result, pixels, size);
		FreeImage_Unload(bitmap);
		
		return result;
	}

	byte* GELoadImage(const String& filename, uint* width, uint* height, uint* bits, bool flipY)
	{
		return GELoadImage(filename.c_str(), width, height, bits, flipY);
	}

}