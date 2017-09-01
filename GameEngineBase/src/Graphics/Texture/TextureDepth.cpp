#include "ge.h"
#include "TextureDepth.h"
#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	TextureDepth* TextureDepth::Create(uint width, uint height)
	{
		TextureDepth* thisTD = genew TextureDepth();
		thisTD->m_Width = width;
		thisTD->m_Height = height;
		thisTD->m_Instance = API::APITextureDepth::Create(width, height);
		return thisTD;
	}

	bool TextureDepth::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;
		this->m_Instance = API::APITextureDepth::Create(m_Width, m_Height);
	}

} }