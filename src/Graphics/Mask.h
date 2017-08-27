#pragma once

#include "Common.h"
#include "Math/Maths.h"
#include "Texture/Texture2D.h"

namespace gebase { namespace graphics {

	struct GE_API Mask
	{
		Texture2D* texture;
		math::Matrix4f transform;

		Mask(Texture2D* texture, const math::Matrix4f& transform = *math::Matrix4f::initIdentity()) : texture(texture), transform(transform)
		{
			this->transform = *math::Matrix4f::initScale((float)texture->getWidth() / (float)texture->getHeight(), 1.0f, 1.0f);
		}
	};

} }