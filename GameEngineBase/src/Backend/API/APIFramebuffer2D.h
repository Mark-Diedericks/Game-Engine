#pragma once

#include "APIFramebuffer.h"

namespace gebase { namespace graphics { namespace API {

	class APIFramebuffer2D : public APIFramebuffer
	{
	protected:
		math::Vector4f m_ClearColor;
	public:
		virtual void setClearColor(const math::Vector4f& color) = 0;
		virtual const math::Vector4f& getColor() const = 0;

		virtual byte* getPixelData() = 0;
		virtual void setData(const void* data) = 0;

		static APIFramebuffer2D* Create(uint width, uint height);
	};

} } }