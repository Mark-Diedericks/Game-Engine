#pragma once

#include "Common.h"

#include "Graphics/Shader/Shader.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Camera/Camera.h"

#include "Math/Maths.h"

namespace gebase { namespace debug {

	enum class DebugRenderMeshFlags
	{
		NONE = 0,
		WIREFRAME = BIT(0),
		NORMALS = BIT(1),
		BINORMALS = BIT(2),
		TANGETS = BIT(3)
	};

	class DebugRenderer
	{
	private:
		DebugRenderer() {}
		GE_DEBUG_METHOD_V(static void DrawLineInternal(const math::Vector3f& start, const math::Vector3f& end, uint color))
	public:
		GE_DEBUG_METHOD_V(static void Init())
		GE_DEBUG_METHOD_V(static void Shutdown())
		GE_DEBUG_METHOD_V(static void Present())

		GE_DEBUG_METHOD_V(static void DrawLine(const math::Vector3f& start, const math::Vector3f& end, uint color = 0xffffffff))
		GE_DEBUG_METHOD_V(static void DrawMesh(const graphics::Mesh* mesh, DebugRenderMeshFlags flags, const math::Matrix4f& transform = math::Matrix4f::Identity()))

		GE_DEBUG_METHOD_V(static void setCamera(graphics::Camera* camera))
	};

} }

