#pragma once

#include "Pch.h"
#include "Common.h"

#include "Shader.h"

namespace gebase { namespace graphics {

	class GE_API ShaderManager
	{
	private:
		static std::vector<Shader*> s_Shaders;
		ShaderManager() { }
	public:
		static Shader* Add(Shader* shader);
		static Shader* Get(const String& name);
		static void Clean();

		static void Reload(const String& name);
		static void Reload(const Shader* shader);
	};

} }