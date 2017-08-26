#include "Pch.h"
#include "PostEffects.h"

namespace gebase { namespace graphics {

	PostEffects::PostEffects()
	{

	}

	PostEffects::~PostEffects()
	{

	}

	void PostEffects::Push(PostEffectsPass* pass)
	{
		m_Passes.push_back(pass);
	}

	void PostEffects::Pop()
	{
		m_Passes.pop_back();
	}


	void PostEffects::RenderPostEffects(Framebuffer* source, Framebuffer* target, VertexArray* quad, IndexBuffer* indicies)
	{
		target->Bind();

		quad->Bind();
		indicies->Bind();

		for (PostEffectsPass* pass : m_Passes)
			pass->RenderPass(target);

		indicies->Unbind();
		quad->Unbind();
	}
} }