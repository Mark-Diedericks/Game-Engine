#pragma once

#include "Common.h"

namespace gebase { namespace graphics {

	class Renderer3D;

	class GE_API IRenderable
	{
	private:
		virtual void Render(Renderer3D& renderer) = 0;
	};

} }