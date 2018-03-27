#include "ge.h"
#include "Common.h"
#include "Shader.h"
#include "ShaderManager.h"

#include "Graphics/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLShader.h"
#include "Backend/D3D/11/DX11Shader.h"

namespace gebase { namespace graphics {

	std::map<Shader*, Shader*> Shader::s_APIChangeMap;
	std::vector<Shader*> Shader::s_Current;

	const Shader* Shader::s_CurrentlyBound = nullptr;

	Shader* Shader::CreateFromFile(const ShaderDeclaration& shader, void* address)
	{
		String gl_src = VirtualFileSystem::Get()->ReadFileString(shader.opengl);
		String dx11_src = VirtualFileSystem::Get()->ReadFileString(shader.d3d11);

		ShaderSource source = { gl_src, "", dx11_src, "" };
		return CreateFromSource(shader, source, address);
	}

	bool Shader::TryCompileFromFile(const ShaderDeclaration& shader, String& error)
	{
		String gl_src = VirtualFileSystem::Get()->ReadFileString(shader.opengl);
		String dx11_src = VirtualFileSystem::Get()->ReadFileString(shader.d3d11);

		ShaderSource source = { gl_src, "", dx11_src, "" };

		return TryCompile(source, error);
	}

	bool Shader::TryCompile(const ShaderSource& source, String& error)
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

	Shader* Shader::CreateFromSource(const ShaderDeclaration& shader, const ShaderSource& source, void* address)
	{
		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: return (address != nullptr) ? new(address) GLShader(shader, source) : genew GLShader(shader, source);
			//case RenderAPI::VULKAN: return genew VKShader(shader.name, source);
		case RenderAPI::D3D11: return (address != nullptr) ? new(address) DX11Shader(shader, source) : genew DX11Shader(shader, source);
			//case RenderAPI::D3D12: return genew DX12Shader(shader.name, source);
		}

		return nullptr;
	}

	Shader* Shader::ConvertRenderAPI(RenderAPI api, Shader* original)
	{
		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		Shader* shader = CreateFromSource(original->getDeclaration(), original->getSource());

		AddRenderAPIChange(original, shader);

		return shader;
	}

	void Shader::PrepareRenderAPIChange(RenderAPI newApi)
	{
	}

	void Shader::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<Shader*, Shader*>::iterator it;
		for (it = s_APIChangeMap.begin(); it != s_APIChangeMap.end(); it++)
		{
			//gedel ((Shader*)it->first);
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLShader*)it->first); break;
				//case RenderAPI::VULKAN: gedel (VKShader*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11Shader*)it->first); break;
				//case RenderAPI::D3D12: gedel ((DX12Shader*)it->first); break;
			}
		}

		s_APIChangeMap.clear();
	}

} }