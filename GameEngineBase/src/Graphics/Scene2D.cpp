#include "ge.h"
#include "Scene2D.h"

#include "Application/Application.h"
#include "Debug/DebugRenderer.h"

namespace gebase
{
	Scene2D::Scene2D()
	{
		uint width = Application::getApplication().getWindowWidth();
		uint height = Application::getApplication().getWindowHeight();
		float aspect = (float)width / (float)height;

		m_Camera = genew graphics::OrthographicCamera(math::Matrix4f::initOrthographic(-10.0f * aspect, 10.0f * aspect, 10.0f, -10.0f, -1.0f, 1.0f));
		m_Renderer = genew graphics::Renderer2D(width, height);
		m_Renderer->setCamera(m_Camera);
	}

	Scene2D::Scene2D(const math::Matrix4f& projectionMatrix) : m_Camera(genew graphics::OrthographicCamera(projectionMatrix))
	{
		uint width = Application::getApplication().getWindowWidth();
		uint height = Application::getApplication().getWindowHeight();

		m_Renderer = genew graphics::Renderer2D(width, height);
		m_Renderer->setCamera(m_Camera);
	}

	Scene2D::~Scene2D()
	{
		gedel m_Camera;
	}

	void Scene2D::Add(entity::Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	void Scene2D::OnUpdate()
	{

	}

	void Scene2D::OnRender(graphics::Renderer2D& renderer)
	{

	}

	void Scene2D::OnRender()
	{
		graphics::Camera* camera = m_Camera;
		camera->Update(Application::getApplication().getFrT());

		debug::DebugRenderer::setCamera(camera);

		m_Renderer->Begin();

		for (entity::Entity* entity : m_Entities)
		{
			entity::component::SpriteComponent* sprite = entity->getComponent<entity::component::SpriteComponent>();

			if (sprite)
			{
				entity::component::TransformComponent* transform = entity->getComponent<entity::component::TransformComponent>();
				if (!transform)
				{
					std::cout << "[Scene2D] OnRender() - Entity does not have a transform component!" << std::endl;
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				sprite->sprite->Submit(m_Renderer);;
			}
		}

		OnRender(*m_Renderer);

		m_Renderer->End();
		m_Renderer->Present();
		return;
	}

}