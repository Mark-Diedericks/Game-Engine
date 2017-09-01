#include "ge.h"
#include "Common.h"
#include "Shader.h"

#include "Graphics/Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	Shader* Shader::CreateFromFile(const API::ShaderDeclaration& shader, void* address)
	{
		Shader* thisS = genew Shader();
		thisS->m_Instance = API::APIShader::CreateFromFile(shader, address);
		return thisS;
	}

	Shader* Shader::CreateFromSource(const API::ShaderDeclaration& shader, const String& source)
	{
		Shader* thisS = genew Shader();
		thisS->m_Declaration = shader;
		thisS->m_Source = source;
		thisS->m_Instance = API::APIShader::CreateFromSource(shader, source);
		return thisS;
	}

	bool Shader::EmployRenderAPI(RenderAPI api)
	{
		gedel this->m_Instance;

		if(m_Source.length() > 0)
			this->m_Instance = API::APIShader::CreateFromSource(m_Declaration, m_Source);

		return true;
	}

} }