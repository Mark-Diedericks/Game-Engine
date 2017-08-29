#pragma once

#include "ge.h"
#include "Common.h"

#include "Math/Maths.h"
#include "System/VirtualFileSystem.h"

#include "APIShaderUniform.h"
#include "APIShaderResource.h"
#include "APIShaderDeclaration.h"

namespace gebase { namespace graphics { namespace API {

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_MASK_US_INDEX	2
#define SHADER_TID_INDEX		3
#define SHADER_MID_INDEX		4
#define SHADER_COLOR_INDEX		5

#define SHADER_UNIFORM_PROJ_MATRIX_NAME		"sys_ProjectionMatrix"
#define SHADER_UNIFORM_VIEW_MATRIX_NAME		"sys_ViewMatrix"
#define SHADER_UNIFORM_MODEL_MATRIX_NAME	"sys_ModelMatrix"

	class APIShader
	{
	public:
		static const APIShader* s_CurrentlyBound;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void setVSSystemUniformBuffer(byte* data, uint size, uint slot = 0) = 0;
		virtual void setFSSystemUniformBuffer(byte* data, uint size, uint slot = 0) = 0;

		virtual void setVSUserUniformBuffer(byte* data, uint size) = 0;
		virtual void setFSUserUniformBuffer(byte* data, uint size) = 0;

		virtual const String& getName() const = 0;
		virtual const String& getFilepath() const = 0;

		virtual const ShaderResourceList& getResources() const = 0;

		virtual const ShaderUniformBufferList& getVSSystemUniforms() const = 0;
		virtual const ShaderUniformBufferList& getFSSystemUniforms() const = 0;

		virtual const ShaderUniformBufferDeclaration* getVSUserUniformBuffer() const = 0;
		virtual const ShaderUniformBufferDeclaration* getFSUserUniformBuffer() const = 0;

		static bool GE_API TryCompile(const String& source, String& error);
		static bool GE_API TryCompileFromFile(const ShaderDeclaration& shader, String& error);

		static APIShader* CreateFromFile(const ShaderDeclaration& shader, void* address = nullptr);
		static APIShader* CreateFromSource(const ShaderDeclaration& shader, const String& source);
	};

} } }