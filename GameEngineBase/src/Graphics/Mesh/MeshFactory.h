#pragma once

#include "Common.h"
#include "Types.h"
#include "Math/Maths.h"

#include "Mesh.h"
#include "Graphics/Material/Material.h"
#include "Graphics/Buffer/VertexArray.h"

namespace gebase { namespace graphics { namespace MeshFactory {

	GE_API Mesh* CreateQuad(float x, float y, float width, float height, MaterialInstance* material);
	GE_API Mesh* CreateQuad(math::Vector2f& position, math::Vector2f& size, MaterialInstance* material);


	GE_API Mesh* CreateCube(float size, MaterialInstance* material);
	GE_API Mesh* CreatePlane(float width, float height, const math::Vector3f& normal, MaterialInstance* material);

} } }