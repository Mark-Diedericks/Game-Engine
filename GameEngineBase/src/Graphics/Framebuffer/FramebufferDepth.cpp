#include "ge.h"
#include "FramebufferDepth.h"
#include "Graphics/Context.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {
	
	FramebufferDepth* FramebufferDepth::Create(uint width, uint height)
	{
		FramebufferDepth* thisFB = genew FramebufferDepth();
		thisFB->m_Width = width;
		thisFB->m_Height = height;
		thisFB->m_Instance = API::APIFramebufferDepth::Create(width, height);
		return thisFB;
	}

	bool FramebufferDepth::EmployRenderAPI(RenderAPI api)
	{
		if (current == api)
			return true;

		current = api;

		gedel this->m_Instance;
		this->m_Instance = API::APIFramebufferDepth::Create(m_Width, m_Height);
		return true;
	}

} }