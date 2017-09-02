#pragma once

#include "CustomString.h"

namespace gebase { namespace graphics { namespace API {

	struct GE_API ShaderDeclaration
	{
		String name;
		String opengl;
		String vulkan;
		String d3d11;
		String d3d12;
	};

	struct GE_API ShaderSource
	{
		String opengl;
		String vulkan;
		String d3d11;
		String d3d12;
	};

} } }