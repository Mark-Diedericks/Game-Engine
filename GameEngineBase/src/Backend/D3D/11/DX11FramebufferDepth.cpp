#include "ge.h"
#include "Common.h"
#include "DX11FramebufferDepth.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	DX11FramebufferDepth::DX11FramebufferDepth(uint width, uint height) : FramebufferDepth(0), m_Width(width), m_Height(height) { Init(); }

	DX11FramebufferDepth::~DX11FramebufferDepth() { }

	void DX11FramebufferDepth::Init()
	{

	}

	void DX11FramebufferDepth::Bind() const
	{

	}

	void DX11FramebufferDepth::Unbind() const
	{

	}

	void DX11FramebufferDepth::Clear()
	{

	}

	void DX11FramebufferDepth::getPixelData(uint16* data)
	{
		data = nullptr;
	}

	void DX11FramebufferDepth::setData(const uint16* data)
	{

	}

} } 