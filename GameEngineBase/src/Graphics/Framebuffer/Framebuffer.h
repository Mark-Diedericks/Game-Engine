#pragma once

#include "Common.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/BackendDependency/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	class GE_API Framebuffer : public IRenderAPIDependant
	{
	protected:
		Framebuffer(uint loadType) : IRenderAPIDependant(RenderObjectType::Framebuffer, loadType) { }
		virtual void Init() {};
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Clear() = 0;

		virtual Texture* getTexture() const = 0;
		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;
	};

} }