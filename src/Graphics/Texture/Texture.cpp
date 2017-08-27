#include "ge.h"
#include "Texture.h"

namespace gebase { namespace graphics {

	API::TextureWrap Texture::s_WrapMode = API::TextureWrap::CLAMP;
	API::TextureFilter Texture::s_FilterMode = API::TextureFilter::LINEAR;

} }