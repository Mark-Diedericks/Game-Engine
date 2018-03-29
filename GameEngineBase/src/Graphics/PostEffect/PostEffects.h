#pragma once

#include "Common.h"
#include "Types.h"

#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "PostEffectsPass.h"

namespace gebase { namespace graphics {

	class GE_API PostEffects : public IRenderAPIDependantContainer
	{
	private:
		std::vector<PostEffectsPass*> m_Passes;
	public:
		PostEffects();
		~PostEffects();

		void Push(PostEffectsPass* pass);
		void Pop();
		
		void RenderPostEffects(Framebuffer* source, Framebuffer* target, VertexArray* quad, IndexBuffer* indices);

		bool EmployRenderAPIShader(RenderAPI api) override;
		bool EmployRenderAPITexture2D(RenderAPI api) override;
		bool EmployRenderAPITextureCube(RenderAPI api) override;
		bool EmployRenderAPITextureDepth(RenderAPI api) override;
		bool EmployRenderAPIFramebuffer2D(RenderAPI api) override;
		bool EmployRenderAPIFramebufferDepth(RenderAPI api) override;
		bool EmployRenderAPIIndexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexBuffer(RenderAPI api) override;
		bool EmployRenderAPIVertexArray(RenderAPI api) override;
 	};

} }