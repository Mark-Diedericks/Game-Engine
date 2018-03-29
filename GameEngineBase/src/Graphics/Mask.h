#pragma once

#include "Common.h"
#include "Math/Maths.h"
#include "Texture/Texture2D.h"

namespace gebase { namespace graphics {

	struct GE_API Mask : public IRenderAPIDependantContainer
	{
		Texture2D* texture;
		math::Matrix4f transform;

		Mask(Texture2D* texture, const math::Matrix4f& transform = math::Matrix4f::Identity()) : texture(texture), transform(transform)
		{
			this->transform = math::Matrix4f::Scale((float)texture->getWidth() / (float)texture->getHeight(), 1.0f, 1.0f);
		}

		inline bool EmployRenderAPIShader(RenderAPI api) override { return true; }
		inline bool EmployRenderAPITexture2D(RenderAPI api) override { texture = Texture2D::ConvertRenderAPI(api, texture); return true; }
		inline bool EmployRenderAPITextureCube(RenderAPI api) override { return true; }
		inline bool EmployRenderAPITextureDepth(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIFramebuffer2D(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIFramebufferDepth(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIIndexBuffer(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIVertexBuffer(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIVertexArray(RenderAPI api) override { return true; }
	};

} }