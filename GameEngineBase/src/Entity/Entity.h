#pragma once

#include "ge.h"
#include "Common.h"
#include "Math/Maths.h"
#include "Component/Components.h"

namespace gebase { namespace entity {

	class GE_API Entity
	{
	protected:
		std::unordered_map<component::ComponentType*, component::Component*> m_Components;
		bool m_Sprite;
		bool m_Mesh;
		bool m_Transform;
	private:
		template <typename T>
		const T* getComponentInternal() const
		{
			component::ComponentType* type = T::getStaticType();
			auto it = m_Components.find(type);

			if (it == m_Components.end())
				return nullptr;

			return (T*)it->second;
		}
	public:
		Entity();
		Entity(graphics::Sprite* sprite, const math::Matrix4f& transform = math::Matrix4f::Identity());
		Entity(graphics::Mesh* mesh, const math::Matrix4f& transform = math::Matrix4f::Identity());

		void AddComponent(component::Component* component);

		inline const bool HasSprite() const { return m_Sprite; }
		inline const bool HasMesh() const { return m_Mesh; }
		inline const bool HasTransform() const { return m_Transform; }

		template <typename T>
		const T* getComponent() const
		{
			return (T*)getComponentInternal<T>();
		}

		template <typename T>
		T* getComponent()
		{
			return (T*)getComponentInternal<T>();
		}
	};

} }