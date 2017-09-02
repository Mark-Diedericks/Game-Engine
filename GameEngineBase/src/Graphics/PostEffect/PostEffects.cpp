#include "ge.h"
#include "PostEffects.h"

namespace gebase { namespace graphics {

	PostEffects::PostEffects() : IRenderAPIDependant(RenderObjectType::Material)
	{

	}

	PostEffects::~PostEffects()
	{

	}

	bool PostEffects::EmployRenderAPI(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if(!m_Passes[i]->EmployRenderAPI(api))
				return false;

		return true;
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