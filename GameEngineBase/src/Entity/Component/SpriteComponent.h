#pragma once

#include "Component.h"
#include "Graphics/Sprite.h"

namespace gebase { namespace entity { namespace component {

	class GE_API SpriteComponent : public Component
	{
	public:
		gebase::graphics::Sprite* sprite;
		SpriteComponent(gebase::graphics::Sprite* sprite);

		static ComponentType* getStaticType()
		{
			static ComponentType type({"Sprite"});
			return &type;
		}

		inline virtual ComponentType* getType() const override { return getStaticType(); }
	};

} } }