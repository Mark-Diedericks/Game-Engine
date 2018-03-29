#pragma once

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Light/LightSetup.h"
#include "IRenderer.h"

namespace gebase { namespace graphics {

	typedef std::vector<RenderCommand> CommandQue;
	typedef std::vector<RendererUniform> SystemUniformList;

	class GE_API Renderer3D : public IRenderer
	{
	protected:
		uint m_ScreenBufferWidth;
		uint m_ScreenBufferHeight;
		CommandQue m_CommandQue;
		SystemUniformList m_SystemUniforms;
		bool m_Began;

		Renderer3D() : IRenderer() { }
	public:
		virtual ~Renderer3D() {}

		virtual void Init() = 0;
		
		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void BeginScene(Camera* camera) = 0;
		virtual void EndScene() = 0;

		virtual void Present() = 0;

		virtual void Submit(const RenderCommand& command) = 0;
		virtual void SubmitMesh(Mesh* mesh, const math::Matrix4f& transform) = 0;
		virtual void SubmitLightSetup(const LightSetup& lightSetup) = 0;

		virtual void setScreenBufferSize(uint width, uint height) { m_ScreenBufferWidth = width, m_ScreenBufferHeight = height; }
	};

} }