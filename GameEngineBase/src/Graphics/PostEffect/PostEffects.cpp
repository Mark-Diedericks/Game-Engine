#include "ge.h"
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

	bool PostEffects::EmployRenderAPIShader(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPIShader(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPITexture2D(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPITexture2D(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPITextureCube(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPITextureCube(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPITextureDepth(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPITextureDepth(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPIFramebuffer2D(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPIFramebufferDepth(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPIIndexBuffer(api))
				return false;

		return true;
	}

	bool PostEffects::EmployRenderAPIVertexArray(RenderAPI api)
	{
		for (uint i = 0; i < m_Passes.size(); i++)
			if (!m_Passes[i]->EmployRenderAPIVertexArray(api))
				return false;

		return true;
	}

} }