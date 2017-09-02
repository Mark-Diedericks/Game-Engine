#include "ge.h"
#include "Common.h"
#include "Shader.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	Shader* Shader::CreateFromFile(const API::ShaderDeclaration& shader, void* address)
	{
		Shader* thisS = genew Shader();

		String gl_src = VirtualFileSystem::Get()->ReadFileString(shader.opengl);
		String dx11_src = VirtualFileSystem::Get()->ReadFileString(shader.d3d11);

		API::ShaderSource source = { gl_src, "", dx11_src, "" };

		thisS->m_Source = source;
		thisS->m_Instance = API::APIShader::CreateFromFile(shader, source, address);
		return thisS;
	}

	Shader* Shader::CreateFromSource(const API::ShaderDeclaration& shader, const API::ShaderSource& source)
	{
		Shader* thisS = genew Shader();
		thisS->m_Declaration = shader;
		thisS->m_Source = source;
		thisS->m_Instance = API::APIShader::CreateFromSource(shader, source);
		return thisS;
	}

	bool Shader::TryCompileFromFile(const API::ShaderDeclaration& shader, String& error)
	{
		String gl_src = VirtualFileSystem::Get()->ReadFileString(shader.opengl);
		String dx11_src = VirtualFileSystem::Get()->ReadFileString(shader.d3d11);

		API::ShaderSource source = { gl_src, "", dx11_src, "" };
		return API::APIShader::TryCompileFromFile(shader, source, error);
	}

	bool Shader::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;

		this->m_Instance = API::APIShader::CreateFromSource(m_Declaration, m_Source);

		return true;
	}

} }