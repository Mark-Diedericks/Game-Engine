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

		bool EmployRenderAPI(RenderAPI api) override;
		void RenderPass(Framebuffer* target);
	};

} }
