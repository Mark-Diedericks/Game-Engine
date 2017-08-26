#pragma once

#include "Common.h"
#include "Shader.h"

namespace gebase { namespace graphics { namespace ShaderFactory {

	GE_API Shader* SimpleShader();
	GE_API Shader* BasicLightShader();
	GE_API Shader* BatchRendererShader();

	GE_API Shader* GeometryShader();

	GE_API Shader* DebugShader();

} } }