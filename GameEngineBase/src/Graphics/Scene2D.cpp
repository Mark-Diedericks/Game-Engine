#include "ge.h"
#include "Scene2D.h"

#include "Application/Application.h"
#include "Debug/DebugRenderer.h"
#include "Utils\LogUtil.h"

namespace gebase
{
	Scene2D::Scene2D()
	{
		uint width = Application::getApplication().getWindowWidth();
		uint height = Application::getApplication().getWindowHeight();
		float aspect = (float)width / (float)height;

		m_Camera = genew graphics::OrthographicCamera(math::Matrix4f::Orthographic(-10.0f * aspect, 10.0f * aspect, -10.0f, 10.0f, -1.0f, 1.0f));
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
		gedel m_Renderer;
	}

	void Scene2D::Add(entity::Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	void Scene2D::OnUpdate(const float delta)
	{
		m_Camera->Update(delta);
	}

	void Scene2D::OnRender(graphics::Renderer2D& renderer)
	{

	}

	void Scene2D::OnRender()
	{
		debug::DebugRenderer::setCamera(m_Camera);

		for (entity::Entity* entity : m_Entities)
		{
			if (entity->HasSprite())
			{
				entity::component::SpriteComponent* sprite = entity->getComponent<entity::component::SpriteComponent>();
				entity::component::TransformComponent* transform = entity->getComponent<entity::component::TransformComponent>();
				if (!transform)
				{
					utils::LogUtil::WriteLine("ERROR", "[Scene2D] OnRender() - Entity does not have a transform component!");
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				sprite->sprite->Submit(m_Renderer);
			}
		}

		OnRender(*m_Renderer);
		return;
	}

}