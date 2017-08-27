#pragma once

#include "Common.h"
#include "Graphics/Texture/Texture.h"
#include "Backend/API/APIFramebuffer.h"

namespace gebase { namespace graphics {

	class GE_API Framebuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Clear() = 0;

		virtual Texture* getTexture() const = 0;
		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;

		virtual API::APIFramebuffer* getInstance() = 0;
	};

} }