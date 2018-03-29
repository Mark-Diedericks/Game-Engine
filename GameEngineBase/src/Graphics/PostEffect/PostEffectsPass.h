#pragma once

#include "Common.h"
#include "Types.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Material/Material.h"

namespace gebase { namespace graphics {

	class GE_API PostEffectsPass : public IRenderAPIDependantContainer
	{
	private:
		Material* m_Material;
	public:
		PostEffectsPass(Shader* shader);
		~PostEffectsPass();

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;

		void RenderPass(Framebuffer* target);
	};

} }
