#include "ge.h"
#include "ShaderFactory.h"
#include "Graphics/Context/Context.h"

namespace gebase{ namespace graphics { namespace ShaderFactory {

#ifdef GE_PLATFORM_WINDOWS

	static const char* s_SimpleShader = 
#include "default/Simple.shader"
		;

	static const char* s_BasicLightShader = 
#include "default/BasicLight.shader"
		;

	static const char* s_GeometryShader = 
#include "default/GeometryPass.shader"
		;

	static const char* s_DebugShader = 
#include "default/Debug.Shader"
		;

	//GL-VK-D3D
	static const char* s_BatchRendererShaderGL = 
#include "Backend/GL/Shaders/BatchRenderer.shader"
		;

	static const char* s_BatchRendererShaderVK =
#include "Backend/GL/Shaders/BatchRenderer.shader"
		;

	static const char* s_BatchRendererShaderD3D = 
#include "Backend/D3D/Shaders/BatchRenderer.hlsl"
		;
#else
#error No GLES Shaders
#endif // PLATFORM_WINDOWS


	Shader* BatchRendererShader() 
	{ 
		return Shader::CreateFromSource({ "Batch Renderer" }, { s_BatchRendererShaderGL, s_BatchRendererShaderVK, s_BatchRendererShaderD3D, s_BatchRendererShaderD3D });
	}

	Shader* SimpleShader() { return Shader::CreateFromSource({ "Simple Shader" }, { s_SimpleShader, s_SimpleShader, s_SimpleShader, s_SimpleShader }); }
	Shader* BasicLightShader() { return Shader::CreateFromSource({ "Basic Light Shader" }, { s_BasicLightShader, s_BasicLightShader, s_BasicLightShader, s_BasicLightShader }); }
	Shader* GeometryShader() { return Shader::CreateFromSource({ "Geometry Shader" }, { s_GeometryShader, s_GeometryShader, s_GeometryShader, s_GeometryShader }); }

	Shader* DebugShader() { return Shader::CreateFromSource({ "Debug Shader" }, { s_DebugShader, s_DebugShader, s_DebugShader, s_DebugShader }); }

} } }