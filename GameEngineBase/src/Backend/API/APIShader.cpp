#include "ge.h"
#include "Common.h"
#include "APIShader.h"

#include "Backend/GL/GLShader.h"
#include "Backend/D3D/11/DX11Shader.h"
//#include "Backend/D3D/12/DX12Shader.h"
//#include "Backend/VK/VKShader.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	const APIShader* APIShader::s_CurrentlyBound = nullptr;

	bool APIShader::TryCompile(const API::ShaderSource& source, String& error)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return GLShader::TryCompile(source.opengl, error);
		//case RenderAPI::VULKAN: return VKShader::TryCompile(source, error);
		case RenderAPI::D3D11: return DX11Shader::TryCompile(source.d3d11, error);
		//case RenderAPI::D3D12: return DX12Shader::TryCompile(source, error);
		}

		return nullptr;
	}

	bool APIShader::TryCompileFromFile(const ShaderDeclaration& shader, const API::ShaderSource& source, String& error)
	{
		return TryCompile(source, error);
	}

	APIShader* APIShader::CreateFromSource(const ShaderDeclaration& shader, const API::ShaderSource& source)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLShader(shader.name, source.opengl);
		//case RenderAPI::VULKAN: return genew VKShader(shader.name, source);
		case RenderAPI::D3D11: return genew DX11Shader(shader.name, source.d3d11);
		//case RenderAPI::D3D12: return genew DX12Shader(shader.name, source);
		}

		return nullptr;
	}


} } }