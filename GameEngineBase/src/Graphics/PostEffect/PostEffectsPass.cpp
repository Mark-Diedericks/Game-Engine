#include "ge.h"
#include "PostEffectsPass.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	struct PostEffectsPassShader
	{
		math::Matrix4f& pr_matrix;
		Texture* texture;
	};

	PostEffectsPass::PostEffectsPass(Shader* shader) : IRenderAPIDependant(RenderObjectType::Material)
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

	bool PostEffectsPass::EmployRenderAPI(RenderAPI api)
	{
		return m_Material->EmployRenderAPI(api);
	}

} }