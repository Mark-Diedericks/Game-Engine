#pragma once

#include "Common.h"

#include "Math/Maths.h"
#include "APITexture.h"
#include "Graphics/Texture/Texture.h"

namespace gebase { namespace graphics { namespace API {

	class APIFramebuffer
	{
	protected:
		virtual void Init() {};
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Clear() = 0;

		virtual Texture* getTexture() const = 0;
		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;
	};

} } }