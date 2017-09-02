#pragma once

#include "Common.h"
#include "Renderer3D.h"
#include "Graphics/GBuffer.h"

namespace gebase { namespace graphics {

	class DeferredRenderer : public Renderer3D
	{
	private:
		uint m_Width;
		uint m_Height;
		GBuffer* m_GBuffer;
		Shader* m_GeometryPassShader;

		void setSystemUniforms(Shader* shader);
		void setRequiredUniforms(Shader* sahder, const std::vector<RendererUniform>& uniforms);
	public:
		DeferredRenderer();
		DeferredRenderer(uint width, uint height);
		~DeferredRenderer();

		bool PreEmployRenderAPI() override;
		bool EmployRenderAPI(RenderAPI api) override;

		void Init() override;
		void Begin() override;
		void End() override;
		void BeginScene(Camera* camera) override;
		void EndScene() override;
		void Present() override;

		void Submit(const RenderCommand& command) override;
		void SubmitMesh(Mesh* mesh, const math::Matrix4f& transform) override;
		void SubmitLightSetup(const LightSetup& lightSetup) override;

		inline const GBuffer& getGBuffer() const { return *m_GBuffer; }
	};

} }