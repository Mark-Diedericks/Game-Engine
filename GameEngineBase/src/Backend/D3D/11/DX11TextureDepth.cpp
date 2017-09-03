#include "ge.h"
#include "Common.h"
#include "DX11TextureDepth.h"

namespace gebase { namespace graphics { namespace API {

	DX11TextureDepth::DX11TextureDepth(uint width, uint height) : m_Width(width), m_Height(height) { Init(); }
	
	DX11TextureDepth::~DX11TextureDepth() { }

	void DX11TextureDepth::Init()
	{

	}

	void DX11TextureDepth::Bind(uint slot) const
	{

	}

	void DX11TextureDepth::Unbind(uint slot) const
	{

	}

	void DX11TextureDepth::setData(const uint16* pixels)
	{

	}

	void DX11TextureDepth::getPixelData(uint16* data)
	{
		data = nullptr;
	}

	uint DX11TextureDepth::getSize() const
	{
		return 0;
	}

} } }