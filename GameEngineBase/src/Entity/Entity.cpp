#include "ge.h"
#include "Entity.h"
#include "Common.h"
#include "System/Memory.h"
#include "Component/Components.h"

namespace gebase { namespace entity {

	Entity::Entity() {}

	Entity::Entity(graphics::Mesh* mesh, const math::Matrix4f& transform)
	{
		AddComponent(genew component::MeshComponent(mesh));
		AddComponent(genew component::TransformComponent(transform));

		m_Mesh = mesh != nullptr;
		m_Transform = true;
	}

	Entity::Entity(graphics::Sprite* sprite, const math::Matrix4f& transform)
	{
		AddComponent(genew component::SpriteComponent(sprite));
		AddComponent(genew component::TransformComponent(transform));

		m_Sprite = sprite != nullptr;
		m_Transform = true;
	}

	void Entity::AddComponent(component::Component* component)
	{
		m_Components[component->getType()] = component;

		if (component->getType()->name == "Mesh")
			m_Mesh = true;

		if (component->getType()->name == "Sprite")
			m_Sprite = true;

		if (component->getType()->name == "Transform")
			m_Transform = true;
	}
} }