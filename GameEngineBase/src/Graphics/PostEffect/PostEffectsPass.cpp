#include "ge.h"
#include "PostEffectsPass.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	struct PostEffectsPassShader
	{
		math::Matrix4f& pr_matrix;
		Texture* texture;
	};

	PostEffectsPass::PostEffectsPass(Shader* shader) 
	{
		m_Material = genew Material(shader);
	}

	PostEffectsPass::~PostEffectsPass()
	{

	}

	void PostEffectsPass::RenderPass(Framebuffer* target)
	{
		m_Material->setUniform("pr_matrix", math::Matrix4f::Orthographic(0.0f, (float)target->getWidth(), (float)target->getHeight(), 0.0f, -1.0f, 1.0f));
		m_Material->Bind();
	}

	bool PostEffectsPass::EmployRenderAPIShader(RenderAPI api)
	{
		return m_Material->EmployRenderAPIShader(api);
	}

	bool PostEffectsPass::EmployRenderAPITexture2D(RenderAPI api)
	{
		return m_Material->EmployRenderAPITexture2D(api);
	}

	bool PostEffectsPass::EmployRenderAPITextureCube(RenderAPI api)
	{
		return m_Material->EmployRenderAPITextureCube(api);
	}

	bool PostEffectsPass::EmployRenderAPITextureDepth(RenderAPI api)
	{
		return m_Material->EmployRenderAPITextureDepth(api);
	}

	bool PostEffectsPass::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		return m_Material->EmployRenderAPIFramebuffer2D(api);
	}

	bool PostEffectsPass::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		return m_Material->EmployRenderAPIFramebufferDepth(api);
	}

	bool PostEffectsPass::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		return true;
	}

	bool PostEffectsPass::EmployRenderAPIVertexArray(RenderAPI api)
	{
		return true;
	}

} }