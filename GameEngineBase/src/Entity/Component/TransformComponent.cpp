#include "Pch.h"
#include "TransformComponent.h"

namespace gebase { namespace entity { namespace component {

	TransformComponent::TransformComponent(const math::Matrix4f& transform) : transform(transform) {}

} } }