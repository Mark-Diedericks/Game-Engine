#include "Pch.h"
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

	bool APIShader::TryCompile(const String& source, String& error)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return GLShader::TryCompile(source, error);
		//case RenderAPI::VULKAN: return VKShader::TryCompile(source, error);
		case RenderAPI::D3D11: return DX11Shader::TryCompile(source, error);
		//case RenderAPI::D3D12: return DX12Shader::TryCompile(source, error);
		}

		return nullptr;
	}

	bool APIShader::TryCompileFromFile(const ShaderDeclaration& shader, String& error)
	{
		String filepath = "";

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: filepath = shader.opengl; break;
		case RenderAPI::VULKAN: filepath = shader.vulkan; break;
		case RenderAPI::D3D11: filepath = shader.d3d11; break;
		case RenderAPI::D3D12: filepath = shader.d3d12; break;
		}

		String src = VirtualFileSystem::Get()->ReadFileString(filepath);
		return TryCompile(src, error);
	}

	APIShader* APIShader::CreateFromFile(const ShaderDeclaration& shader, void* address)
	{
		String filepath = "";

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: filepath = shader.opengl; break;
		case RenderAPI::VULKAN: filepath = shader.vulkan; break;
		case RenderAPI::D3D11: filepath = shader.d3d11; break;
		case RenderAPI::D3D12: filepath = shader.d3d12; break;
		}

		String source = VirtualFileSystem::Get()->ReadFileString(filepath);

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return address ? new(address) GLShader(shader.name, source) : genew GLShader(shader.name, source);
		//case RenderAPI::VULKAN: return address ? new(address) VKShader(shader.name, source) : genew VKShader(shader.name, source);
		case RenderAPI::D3D11: return address ? new(address) DX11Shader(shader.name, source) : genew DX11Shader(shader.name, source);
		//case RenderAPI::D3D12: return address ? new(address) DX12Shader(shader.name, source) : genew DX12Shader(shader.name, source);
		}

		return nullptr;
	}

	APIShader* APIShader::CreateFromSource(const ShaderDeclaration& shader, const String& source)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return genew GLShader(shader.name, source);
		//case RenderAPI::VULKAN: return genew VKShader(shader.name, source);
		case RenderAPI::D3D11: return genew DX11Shader(shader.name, source);
		//case RenderAPI::D3D12: return genew DX12Shader(shader.name, source);
		}

		return nullptr;
	}


} } }