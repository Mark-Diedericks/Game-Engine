#include "ge.h"
#include "Texture.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "TextureDepth.h"

namespace gebase { namespace graphics {

	TextureWrap Texture::s_WrapMode = TextureWrap::CLAMP;
	TextureFilter Texture::s_FilterMode = TextureFilter::LINEAR;

	byte Texture::getStrideFromFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:			 return 3;
		case TextureFormat::RGBA:			 return 4;
		case TextureFormat::LUMINANCE_ALPHA: return 4;
		}

		return 4;
	}

	Texture* Texture::ConvertRenderAPI(RenderAPI api, Texture* texture)
	{
		switch (texture->m_Type)
		{
		case TextureType::TEX2D:
			return Texture2D::ConvertRenderAPI(api, (Texture2D*)texture);
		case TextureType::TEXDEPTH:
			return TextureDepth::ConvertRenderAPI(api, (TextureDepth*)texture);
		case TextureType::TEXCUBE:
			return TextureCube::ConvertRenderAPI(api, (TextureCube*)texture);
		}

		return nullptr;
	}

} }