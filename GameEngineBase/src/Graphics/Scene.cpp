#include "ge.h"
#include "Common.h"
#include "Scene.h"

#include "Application/Application.h"
#include "Renderer/Renderer3D.h"
#include "Camera/FPSCamera.h"
#include "Camera/MayaCamera.h"
#include "Debug/DebugRenderer.h"
#include "System/Memory.h"
#include "Utils/LogUtil.h"

namespace gebase {

	Scene::Scene() : m_Camera(genew graphics::FPSCamera(math::Matrix4f::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)))
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
			utils::LogUtil::WriteLine("ERROR", "[Scene] Add() - Entity does not have a transform component");
			entity->AddComponent(genew entity::component::TransformComponent(math::Matrix4f::Identity()));
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
		m_Camera->Update(delta);
	}

	void Scene::Render(graphics::Renderer3D& renderer)
	{
		debug::DebugRenderer::setCamera(m_Camera);

		renderer.Begin();
		renderer.BeginScene(m_Camera);

		for (graphics::LightSetup* ls : m_LightSetupStack)
			renderer.SubmitLightSetup(ls->getLights());

		for (entity::Entity* entity : m_Entities)
		{
			if (entity->HasMesh())
			{
				entity::component::MeshComponent* mesh = entity->getComponent<entity::component::MeshComponent>();
				if (entity->HasTransform())
				{
					entity::component::TransformComponent* transform = entity->getComponent<entity::component::TransformComponent>();
					renderer.SubmitMesh(mesh->mesh, transform->transform);
				}
				else
				{
					renderer.SubmitMesh(mesh->mesh); 
#ifdef GE_DEBUG
					utils::LogUtil::WriteLine("WARNING", "[Scene] Render() - Entity does not have a transform component!");
					__debugbreak();
#endif
				}
			}
		}

		renderer.EndScene();
		renderer.End();
	}
}