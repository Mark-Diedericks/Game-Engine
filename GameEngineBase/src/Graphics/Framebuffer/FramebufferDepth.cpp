#include "Pch.h"
#include "FramebufferDepth.h"
#include "Graphics/Context.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

		FramebufferDepth* FramebufferDepth::Create(uint width, uint height)
		{
			FramebufferDepth* thisFB = genew FramebufferDepth();
			thisFB->m_Instance = API::APIFramebufferDepth::Create(width, height);
			return thisFB;
		}

} }