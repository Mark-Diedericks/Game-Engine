#include "ge.h"
#include "Common.h"
#include "Scene.h"

#include "Renderer/Renderer3D.h"
#include "Camera/FPSCamera.h"
#include "Camera/MayaCamera.h"
#include "Debug/DebugRenderer.h"
#include "System/Memory.h"

namespace gebase {

	Scene::Scene() : m_Camera(genew graphics::FPSCamera(math::Matrix4f::initPerspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)))
	{

	}

	Scene::Scene(graphics::Camera* camera)
	{
		m_Camera = camera;
	}

	Scene::~Scene()
	{
		for (uint i = 0; i < m_Entities.size(); i++)
			gedel m_Entities[i];

		m_Entities.clear();
		gedel m_Camera;
	}

	void Scene::Add(entity::Entity* entity)
	{
		m_Entities.push_back(entity);

		if (!entity->getComponent<entity::component::TransformComponent>())
		{
			std::cout << "[Scene] Add() - Entity does not have a transform component" << std::endl;
			entity->AddComponent(new entity::component::TransformComponent(math::Matrix4f::initIdentity()));
		}
	}

	void Scene::PushLightSetup(graphics::LightSetup* lightSetup)
	{
		m_LightSetupStack.push_back(lightSetup);
	}

	graphics::LightSetup* Scene::PopLightSetup()
	{
		graphics::LightSetup* result = m_LightSetupStack.back();
		m_LightSetupStack.pop_back();
		return result;
	}

	void Scene::setCamera(graphics::Camera* camera)
	{
		m_Camera = camera;
		m_Camera->Focus();
	}

	void Scene::Update(const float delta)
	{

	}

	void Scene::Render(graphics::Renderer3D& renderer)
	{
		graphics::Camera* camera = m_Camera;
		camera->Update(0.0f);

		debug::DebugRenderer::setCamera(camera);

		renderer.Begin();
		renderer.BeginScene(camera);

		for (uint i = 0; i < m_LightSetupStack.size(); i++)
			renderer.SubmitLightSetup(*m_LightSetupStack[i]);

		for (entity::Entity* entity : m_Entities)
		{
			entity::component::MeshComponent* mesh = entity->getComponent<entity::component::MeshComponent>();

			if (mesh)
			{
				entity::component::TransformComponent* transform = entity->getComponent<entity::component::TransformComponent>();
				if (!transform)
				{
					std::cout << "[Scene] Render() - Entity does not have a transform component!" << std::endl;
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				renderer.SubmitMesh(mesh->mesh, transform->transform);
			}
		}

		renderer.EndScene();
		renderer.End();
	}
}