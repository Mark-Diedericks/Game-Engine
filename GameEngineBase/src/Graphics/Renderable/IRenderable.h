#pragma once

#include "Common.h"
#include "IRenderableBase.h"

namespace gebase { namespace graphics {

	class Renderer3D;

	class GE_API IRenderable : public IRenderableBase
	{
	protected:
		IRenderable() : IRenderableBase() { }
	private:
		virtual void Render(Renderer3D& renderer) = 0;
	};

} }