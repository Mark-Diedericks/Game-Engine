#pragma once

#include "Common.h"
#include "Types.h"
#include "String.h"
#include "Math/Maths.h"
#include "Mesh/Mesh.h"
#include "Shader/Shader.h"

namespace gebase { namespace graphics {

	struct GE_API RendererUniform
	{
		String uniform;
		byte* value;
	};

	struct GE_API RenderCommand
	{
		Mesh* mesh;
		math::Matrix4f transform;
		Shader* shader;
		std::vector<RendererUniform> uniforms;
	};

} }