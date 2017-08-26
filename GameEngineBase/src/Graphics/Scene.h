#pragma once

#include <vector>

#include "Types.h"
#include "Entity/Entity.h"
#include "Camera/Camera.h"
#include "Light/LightSetup.h"

namespace gebase {

	namespace graphics {
		class Renderer3D;
	}

	class GE_API Scene
	{
	private:
		std::vector<entity::Entity*> m_Entities;
		graphics::Camera* m_Camera;
		std::vector<graphics::LightSetup*> m_LightSetupStack;
	public:
		Scene();
		Scene(graphics::Camera* camera);
		~Scene();

		void Add(entity::Entity* entity);
		void PushLightSetup(graphics::LightSetup* lightSetup);
		graphics::LightSetup* PopLightSetup();

		inline graphics::Camera* getCamera() const { return m_Camera; }
		void setCamera(graphics::Camera* camera);

		void Update(const float delta);
		void Render(graphics::Renderer3D& renderer);

		inline const std::vector<entity::Entity*>& getEntities() const { return m_Entities; }
 	};

} 