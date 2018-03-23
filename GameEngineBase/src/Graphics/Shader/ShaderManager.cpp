#include "ge.h"
#include "Common.h"
#include "ShaderManager.h"
#include "System/Memory.h"
#include "Utils/LogUtil.h"

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
				API::ShaderDeclaration path = s_Shaders[i]->getDeclaration();
				String error;

				if (!Shader::TryCompileFromFile(path, error))
				{
					utils::LogUtil::WriteLine("ERROR", "[ShaderManager] Reload() - TryCompileFromFile has failed.");
				}
				else
				{
					s_Shaders[i]->~Shader();
					s_Shaders[i] = Shader::CreateFromFile(path, s_Shaders[i]);
				}

				return;
			}
		}

		utils::LogUtil::WriteLine("ERROR", "[ShaderManager] Reload() - Could not find the shader.");
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

		utils::LogUtil::WriteLine("ERROR", "[ShaderManager] Reload() - Could not find the shader.");
	}

} }