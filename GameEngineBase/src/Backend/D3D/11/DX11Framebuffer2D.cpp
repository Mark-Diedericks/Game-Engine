#include "ge.h"
#include "Common.h"
#include "DX11Framebuffer2D.h"

#include "System/Memory.h"

namespace gebase { namespace graphics {

	DX11Framebuffer2D::DX11Framebuffer2D(uint width, uint height) : Framebuffer2D(0), m_Width(width), m_Height(height) { Init(); }

	DX11Framebuffer2D::~DX11Framebuffer2D() { }

	void DX11Framebuffer2D::Init()
	{

	}

	void DX11Framebuffer2D::Bind() const
	{

	}

	void DX11Framebuffer2D::Unbind() const
	{

	}

	void DX11Framebuffer2D::Clear()
	{

	}

	void DX11Framebuffer2D::getPixelData(byte* data)
	{
		data = nullptr;
	}

	void DX11Framebuffer2D::setData(const byte* data)
	{

	}

} }