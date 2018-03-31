#include "ge.h"
#include "ForwardRenderer.h"

#include "Application/Application.h"
#include "Graphics/Renderer/Renderer.h"
#include "Utils\LogUtil.h"

namespace gebase { namespace graphics {

	enum VSSystemUniformIndices : int32
	{
		VSSystemUniformIndex_ProjectionMatrix	= 0,
		VSSystemUniformIndex_ViewMatrix			= 1,
		VSSystemUniformIndex_ModelMatrix		= 2,
		VSSystemUniformIndex_CameraPosition	= 3,
		VSSystemUniformIndex_Size
	};

	enum FSSystemUniformIndices : int32
	{
		FSSystemUniformIndex_Lights = 0,
		FSSystemUniformIndex_Size
	};

	ForwardRenderer::ForwardRenderer() : Renderer3D(), DISABLE_DEPTHTEST((int)Material::RenderFlags::DISABLE_DEPTH_TEST), MATRIX4F_SIZE(sizeof(math::Matrix4f)), LIGHT_SIZE(sizeof(Light))
	{
		setScreenBufferSize(Application::getApplication().getWindowWidth(), Application::getApplication().getWindowHeight());
		Init();
	}

	ForwardRenderer::ForwardRenderer(uint width, uint height) : Renderer3D(), DISABLE_DEPTHTEST((int)Material::RenderFlags::DISABLE_DEPTH_TEST), MATRIX4F_SIZE(sizeof(math::Matrix4f)), LIGHT_SIZE(sizeof(Light))
	{
		setScreenBufferSize(width, height);
		Init();
	}

	bool ForwardRenderer::PreEmployRenderAPI()
	{
		if (m_Began)
			End();

		Present();
		return true;
	}

	void ForwardRenderer::Init()
	{
		m_CommandQue.reserve(1000);

		//Vertex Shader
		m_VSSystemUniformBufferSize = sizeof(math::Matrix4f) + sizeof(math::Matrix4f) + sizeof(math::Matrix4f) + sizeof(math::Vector3f);
		m_VSSystemUniformBuffer = genew byte[m_VSSystemUniformBufferSize];
		memset(m_VSSystemUniformBuffer, 0, m_VSSystemUniformBufferSize);
		m_VSSystemUniformBufferOffsets.resize(VSSystemUniformIndex_Size);

		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ProjectionMatrix] = 0;
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ViewMatrix] = sizeof(math::Matrix4f);
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ModelMatrix] = sizeof(math::Matrix4f) + sizeof(math::Matrix4f);
		m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_CameraPosition] = sizeof(math::Matrix4f) + sizeof(math::Matrix4f) + sizeof(math::Matrix4f);

		//Fragment Shader
		m_FSSystemUniformBufferSize = sizeof(Light);
		m_FSSystemUniformBuffer = genew byte[m_FSSystemUniformBufferSize];
		memset(m_FSSystemUniformBuffer, 0, m_FSSystemUniformBufferSize);
		m_FSSystemUniformBufferOffsets.resize(FSSystemUniformIndex_Size);

		m_FSSystemUniformBufferOffsets[FSSystemUniformIndex_Lights] = 0;
	}

	void ForwardRenderer::Begin()
	{
		m_Began = true;
		Renderer::setViewport(0, 0, m_ScreenBufferWidth, m_ScreenBufferHeight);

		m_CommandQue.clear();
		m_SystemUniforms.clear();
	}

	void ForwardRenderer::End()
	{
		m_Began = false;
	}

	void ForwardRenderer::BeginScene(Camera* camera)
	{
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ProjectionMatrix], &camera->getProjectionMatrix(), sizeof(math::Matrix4f));
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ViewMatrix], &camera->getViewMatrix(), sizeof(math::Matrix4f));
		memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_CameraPosition], &camera->getPosition(), sizeof(math::Vector3f));
	}

	void ForwardRenderer::EndScene()
	{

	}

	void ForwardRenderer::Submit(const RenderCommand& command)
	{
		m_CommandQue.push_back(command);
	}

	void ForwardRenderer::SubmitMesh(Mesh* mesh, const math::Matrix4f& transform)
	{
		RenderCommand command;
		command.mesh = mesh;
		command.transform = transform;
		command.shader = mesh->getMaterialInstance()->getMaterial()->getShader();
		Submit(command);
	}

	void ForwardRenderer::SubmitLightSetup(const std::vector<Light*>& lights)
	{
		if (lights.size() > 1)
		{
			utils::LogUtil::WriteLine("ERROR", "[ForwardRenderer] SubmitLightSetup() - Only one light is supported currently.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		for (const Light* light : lights)
			memcpy(m_FSSystemUniformBuffer + m_FSSystemUniformBufferOffsets[FSSystemUniformIndex_Lights], light, LIGHT_SIZE);
	}

	void ForwardRenderer::Present()
	{
		for (const RenderCommand& command : m_CommandQue)
		{
			Renderer::setDepthTesting((command.mesh->getMaterialInstance()->getRenderFlags() & DISABLE_DEPTHTEST) == 0);
			memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_ModelMatrix], &command.transform, MATRIX4F_SIZE);

			setSystemUniforms(command.shader);
			command.mesh->Render(*this);
		}
	}

	void ForwardRenderer::setSystemUniforms(Shader* shader)
	{
		if (!shader)
			return;

		shader->Bind();
		shader->setVSSystemUniformBuffer(m_VSSystemUniformBuffer, m_VSSystemUniformBufferSize, 0);
		shader->setFSSystemUniformBuffer(m_FSSystemUniformBuffer, m_FSSystemUniformBufferSize, 0);
	}

} }