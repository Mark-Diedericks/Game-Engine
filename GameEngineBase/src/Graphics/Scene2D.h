#pragma once

#include "Common.h"
#include "Graphics/Camera/OrthographicCamera.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Entity/Entity.h"

namespace gebase {

	class GE_API Scene2D
	{
	private:
		graphics::OrthographicCamera* m_Camera;
		graphics::Renderer2D* m_Renderer;

		std::vector<entity::Entity*> m_Entities;
	public:
		Scene2D();
		Scene2D(const math::Matrix4f& projectionMatrix);
		~Scene2D();

		void Add(entity::Entity* entity);
		void OnRender();

		virtual void OnUpdate();
		virtual void OnRender(graphics::Renderer2D& renderer);

		inline graphics::Renderer2D* getRenderer() { return m_Renderer; }
		inline graphics::OrthographicCamera* getCamera() { return m_Camera; }
	};

}