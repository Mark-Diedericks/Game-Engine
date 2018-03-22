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
		utils::TimeUtil time = utils::TimeUtil();
		time.set();

		if (current == api)
			return true;

		current = api;

		uint16* pixels = genew uint16[this->m_Instance->getSize()];
		this->m_Instance->getPixelData(pixels);

		API::APITextureDepth* inst = API::APITextureDepth::Create(m_Width, m_Height);
		inst->setData(pixels);

		if(pixels)
			gedel[] pixels;

		gedel this->m_Instance;

		if (!inst)
			return false;

		this->m_Instance = inst;

		time.get();

		return true;
	}

} }