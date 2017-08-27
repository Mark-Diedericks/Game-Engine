#pragma once

#include "Component.h"
#include "Common.h"
#include "Math/Maths.h"

namespace gebase { namespace entity { namespace component {

	class GE_API TransformComponent : public Component
	{
	public:
		math::Matrix4f transform;
		TransformComponent(const math::Matrix4f& transform);

		static ComponentType* getStaticType()
		{
			static ComponentType type({ "Transform" });
			return &type;
		}

		inline virtual ComponentType* getType() const override { return getStaticType(); }
	};

} } }