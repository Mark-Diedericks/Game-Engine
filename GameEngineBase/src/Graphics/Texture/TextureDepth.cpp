#include "Pch.h"
#include "TextureDepth.h"
#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		TextureDepth* thisTD = genew TextureDepth();
		thisTD->m_Instance = API::APITextureDepth::Create(width, height);
		return thisTD;
	}

} }