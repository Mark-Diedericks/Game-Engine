#include "Entity.h"
#include "Pch.h"
#include "Common.h"
#include "System/Memory.h"
#include "Component/Components.h"

namespace gebase { namespace entity {

	Entity::Entity() {}

	Entity::Entity(graphics::Mesh* mesh, const math::Matrix4f& transform)
	{
		AddComponent(genew component::MeshComponent(mesh));
		AddComponent(genew component::TransformComponent(transform));
	}

	Entity::Entity(graphics::Sprite* sprite, const math::Matrix4f& transform)
	{
		AddComponent(genew component::SpriteComponent(sprite));
		AddComponent(genew component::TransformComponent(transform));
	}

	void Entity::AddComponent(component::Component* component)
	{
		m_Components[component->getType()] = component;
	}
} }