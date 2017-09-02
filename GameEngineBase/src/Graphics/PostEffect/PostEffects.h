#pragma once

#include "Common.h"
#include "Types.h"

#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "PostEffectsPass.h"

namespace gebase { namespace graphics {

	class GE_API PostEffects : public IRenderAPIDependant
	{
	private:
		std::vector<PostEffectsPass*> m_Passes;
	public:
		PostEffects();
		~PostEffects();

		void Push(PostEffectsPass* pass);
		void Pop();

		bool EmployRenderAPI(RenderAPI api) override;
		void RenderPostEffects(Framebuffer* source, Framebuffer* target, VertexArray* quad, IndexBuffer* indices);
 	};

} }