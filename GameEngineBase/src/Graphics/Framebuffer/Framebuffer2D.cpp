#include "Pch.h"
#include "Framebuffer2D.h"
#include "Graphics/Context.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		Framebuffer2D* thisFB = genew Framebuffer2D();
		thisFB->m_Instance = API::APIFramebuffer2D::Create(width, height);
		return thisFB;
	}

} }