#pragma once

#include "String.h"

namespace gebase { namespace graphics { namespace API {

	struct ShaderDeclaration
	{
		String name;
		String opengl;
		String vulkan;
		String d3d11;
		String d3d12;
	};

} } }