#include "Pch.h"
#include "APITexture.h"

namespace gebase { namespace graphics { namespace API {

	TextureWrap APITexture::s_WrapMode = TextureWrap::CLAMP;
	TextureFilter APITexture::s_FilterMode = TextureFilter::LINEAR;

	byte APITexture::getStrideFromFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:			 return 3;
		case TextureFormat::RGBA:			 return 4;
		case TextureFormat::LUMINANCE_ALPHA: return 4;
		}

		return 0;
	}

} } }