#include "ge.h"
#include "Common.h"
#include "DX11FramebufferDepth.h"

#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	DX11FramebufferDepth::DX11FramebufferDepth(uint width, uint height) : m_Width(width), m_Height(height) { Init(); }

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

	uint16* DX11FramebufferDepth::getPixelData()
	{
		return nullptr;
	}

	void DX11FramebufferDepth::setData(const void* data)
	{

	}

} } }