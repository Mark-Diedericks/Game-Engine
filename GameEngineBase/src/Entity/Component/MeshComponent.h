#pragma once

#include "Component.h"
#include "Graphics/Mesh/Mesh.h"

namespace gebase { namespace entity { namespace component {
	
	class GE_API MeshComponent : public Component
	{
	private:
		//gebase::graphics::Mesh* m_Mesh;
	public:
		//MeshComponent(gebase::graphics::Mesh* mesh);

		//inline void setMesh(gebase::graphics::Mesh* mesh) { m_Mesh = mesh; }
		//inline gebase::graphics::Mesh* getMesh() { return m_Mesh; }

		static ComponentType* getStaticType()
		{
			static ComponentType type({"Mesh"});
			return &type;
		}

		inline virtual ComponentType* getType() const override { return getStaticType(); }
	};

} } }