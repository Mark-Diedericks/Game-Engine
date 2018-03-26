#include "ge.h"
#include "ShaderUniform.h"
#include "Shader.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLShaderUniform.h"
#include "Backend/D3D/11/DX11ShaderUniform.h"

namespace gebase { namespace graphics {
	
	ShaderStruct::~ShaderStruct()
	{
		for (uint i = 0; i < m_Fields.size(); i++)
		{
			//gedel m_Fields[i];

			switch (Application::getApplication().GetPreviousRenderAPI())
			{
			case RenderAPI::OPENGL: gedel((GLShaderUniformDeclaration*)m_Fields[i]); break;
				//case RenderAPI::VULKAN: gedel (VKShaderUniformDeclaration*)m_Fields[i]); break;
			case RenderAPI::D3D11: gedel((DX11ShaderUniformDeclaration*)m_Fields[i]); break;
				//case RenderAPI::D3D12: gedel ((DX12ShaderUniformDeclaration*)m_Fields[i]); break;
			}
		}
	}

} }