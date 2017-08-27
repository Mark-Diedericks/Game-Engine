#pragma once

#include "Component.h"
#include "Graphics/Mesh/Mesh.h"

namespace gebase { namespace entity { namespace component {
	
	class GE_API MeshComponent : public Component
	{
	public:
		gebase::graphics::Mesh* mesh;
		MeshComponent(gebase::graphics::Mesh* mesh);

		static ComponentType* getStaticType()
		{
			static ComponentType type({"Mesh"});
			return &type;
		}

		inline virtual ComponentType* getType() const override { return getStaticType(); }
	};

} } }