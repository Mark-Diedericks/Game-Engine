#pragma once

#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	class APITextureCube : public APITexture
	{
	protected:
		enum class InputFormat
		{
			VERTICAL_CROSS,
			HORIZONTAL_CROSS
		};
	public:
		static APITextureCube* CreateFromFile(const String& name, const byte* pixels, uint width, uint height, uint bits);
		static APITextureCube* CreateFromFiles(const String& name, const byte** sides, uint width, uint height, uint bits);
		static APITextureCube* CreateFromVerticalCross(const String& name, const byte** sides, int32 mips, uint* width, uint* height, uint bits);
	};

} } }