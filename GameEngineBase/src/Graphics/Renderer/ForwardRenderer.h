#pragma once

#include "Common.h"
#include "Renderer3D.h"
#include "Graphics/Shader/Shader.h"

namespace gebase { namespace graphics {

	class GE_API ForwardRenderer : public Renderer3D
	{
	private:
		byte* m_VSSystemUniformBuffer;
		uint m_VSSystemUniformBufferSize;
		std::vector<uint> m_VSSystemUniformBufferOffsets;

		byte* m_FSSystemUniformBuffer;
		uint m_FSSystemUniformBufferSize;
		std::vector<uint> m_FSSystemUniformBufferOffsets;

		void setSystemUniforms(Shader* shader);
	public:
		ForwardRenderer();
		ForwardRenderer(uint width, uint height);

		bool PreEmployRenderAPI() override; 
		
		inline bool EmployRenderAPIShader(RenderAPI api) override { return true; }
		inline bool EmployRenderAPITexture2D(RenderAPI api) override { return true; }
		inline bool EmployRenderAPITextureCube(RenderAPI api) override { return true; }
		inline bool EmployRenderAPITextureDepth(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIFramebuffer2D(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIFramebufferDepth(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIIndexBuffer(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIVertexBuffer(RenderAPI api) override { return true; }
		inline bool EmployRenderAPIVertexArray(RenderAPI api) override { return true; }

		void Init() override;
		void Begin() override;
		void End() override;
		void BeginScene(Camera* camera) override;
		void EndScene() override;
		void Present() override;

		void Submit(const RenderCommand& command) override;
		void SubmitMesh(Mesh* mesh, const math::Matrix4f& transform) override;
		void SubmitLightSetup(const LightSetup& lightSetup) override;
	};

} }