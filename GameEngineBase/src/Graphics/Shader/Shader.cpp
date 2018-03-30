#include "ge.h"
#include "Common.h"
#include "Shader.h"
#include "ShaderManager.h"

#include "Graphics/Context/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLShader.h"
#include "Backend/D3D/11/DX11Shader.h"

namespace gebase { namespace graphics {

	std::map<Shader*, Shader*> Shader::s_APIChangeMapShader;
	std::vector<Shader*> Shader::s_CurrentShader;

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
		case RenderAPI::D3D11: return DX11Shader::TryCompile(source.d3d11, error);
		}

		return nullptr;
	}

	Shader* Shader::CreateFromSource(const ShaderDeclaration& shader, const ShaderSource& source, void* address)
	{
		Shader* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = (address != nullptr) ? new(address) GLShader(shader, source) : genew GLShader(shader, source); break;
		case RenderAPI::D3D11: result = (address != nullptr) ? new(address) DX11Shader(shader, source) : genew DX11Shader(shader, source); break;
		}

		if (result != nullptr)
			s_CurrentShader.push_back(result);

		return result;
	}

	Shader* Shader::ConvertRenderAPI(RenderAPI api, Shader* original)
	{
		if (original == nullptr)
			return nullptr;

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
		std::vector<Shader*> temp(s_CurrentShader);
		s_CurrentShader.clear();
		s_CurrentShader.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void Shader::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<Shader*, Shader*>::iterator it;
		for (it = s_APIChangeMapShader.begin(); it != s_APIChangeMapShader.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLShader*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11Shader*)it->first); break;
			}
		}

		s_APIChangeMapShader.clear();
	}

} }