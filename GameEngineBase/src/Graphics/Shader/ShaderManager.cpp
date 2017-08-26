#include "Pch.h"
#include "Common.h"
#include "ShaderManager.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	std::vector<Shader*> ShaderManager::s_Shaders;

	Shader* ShaderManager::Add(Shader* shader)
	{
		s_Shaders.push_back(shader);
		return shader;
	}

	Shader* ShaderManager::Get(const String& name)
	{
		for (Shader* shader : s_Shaders)
			if (shader->getName() == name)
				return shader;
		return nullptr;
	}

	void ShaderManager::Clean()
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
			gedel[] s_Shaders[i];
	}

	void ShaderManager::Reload(const String& name)
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i]->getName() == name)
			{
				String path = s_Shaders[i]->getFilepath();
				String error;

				if (!API::APIShader::TryCompileFromFile({ name, path, path, path, path }, error))
				{
					std::cout << "[ShaderManager] Reload(String) - TryCompileFromFile has failed." << std::endl;
				}
				else
				{
					s_Shaders[i]->~Shader();
					s_Shaders[i] = Shader::CreateFromFile({ name , path, path, path, path }, s_Shaders[i]);
				}

				return;
			}
		}

		std::cout << "[ShaderManager] Reload(String) - Could not find the shader." << std::endl;
	}

	void ShaderManager::Reload(const Shader* shader)
	{
		for (uint i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i] == shader)
			{
				String name = shader->getName();
				String path = shader->getFilepath();

				s_Shaders[i]->~Shader();
				s_Shaders[i] = Shader::CreateFromFile({ name , path, path, path, path }, s_Shaders[i]);

				return;
			}
		}

		std::cout << "[ShaderManager] Reload(Shader) - Could not find the shader." << std::endl;
	}

} }