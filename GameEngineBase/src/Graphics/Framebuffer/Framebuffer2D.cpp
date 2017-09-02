#include "ge.h"
#include "Framebuffer2D.h"
#include "Graphics/Context.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	Framebuffer2D* Framebuffer2D::Create(uint width, uint height)
	{
		Framebuffer2D* thisFB = genew Framebuffer2D();
		thisFB->m_Width = width;
		thisFB->m_Height = height;
		thisFB->m_Instance = API::APIFramebuffer2D::Create(width, height);
		return thisFB;
	}

	bool Framebuffer2D::EmployRenderAPI(RenderAPI api)
	{
		if (current == api)
			return true;

		current = api;

		gedel this->m_Instance;
		this->m_Instance = API::APIFramebuffer2D::Create(m_Width, m_Height);
		return true;
	}

} }