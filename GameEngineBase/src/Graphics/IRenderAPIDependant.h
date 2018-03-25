#pragma once

#include "Common.h"
#include "Context.h"
#include "Utils\Timer.h"

namespace gebase { namespace graphics {

	enum class RenderObjectType
	{
		Shader = 0,
		Texture,
		Framebuffer,
		Buffer
	};

	class GE_API IRenderAPIDependant
	{
	public:
		RenderObjectType type;
		RenderAPI current;
	protected:
		uint m_LoadType;

		IRenderAPIDependant(RenderObjectType t, uint loadType) { type = t; current = Context::getRenderAPI(); m_LoadType = loadType; }
		inline uint getLoadType() const { return m_LoadType; }
	};

	class GE_API IRenderAPIDependantContainer
	{
	public:
		RenderAPI current;
		virtual bool EmployRenderAPI(RenderAPI api) = 0;
	protected:
		IRenderAPIDependantContainer() { }
	};

} }