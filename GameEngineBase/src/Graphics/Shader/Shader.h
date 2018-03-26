#pragma once

#include "ge.h"
#include "Common.h"

#include "Math/Maths.h"
#include "System/VirtualFileSystem.h"
#include "Graphics/IRenderAPIDependant.h"

#include "ShaderUniform.h"
#include "ShaderResource.h"
#include "ShaderDeclaration.h"

namespace gebase { namespace graphics {

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_MASK_US_INDEX	2
#define SHADER_TID_INDEX		3
#define SHADER_MID_INDEX		4
#define SHADER_COLOR_INDEX		5

#define SHADER_UNIFORM_PROJ_MATRIX_NAME		"sys_ProjectionMatrix"
#define SHADER_UNIFORM_VIEW_MATRIX_NAME		"sys_ViewMatrix"
#define SHADER_UNIFORM_MODEL_MATRIX_NAME	"sys_ModelMatrix"

	class GE_API Shader : public IRenderAPIDependant
	{
	protected:
		Shader(uint loadType) : IRenderAPIDependant(RenderObjectType::Shader, loadType) { }
	public:
		static Shader* CreateFromFile(const ShaderDeclaration& shader, void* address = nullptr);
		static Shader* CreateFromSource(const ShaderDeclaration& shader, const ShaderSource& source, void* adress = nullptr);

		static Shader* ConvertRenderAPI(RenderAPI api, Shader* original);

		virtual ShaderDeclaration getDeclaration() const = 0;
		virtual ShaderSource getSource() const = 0;

		static const Shader* s_CurrentlyBound;

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

		static bool TryCompileFromFile(const ShaderDeclaration& shader, String& error);
		static bool TryCompile(const ShaderSource& source, String& error);
	private:
		static std::map<Shader*, Shader*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(Shader* old, Shader* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(Shader* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline Shader* GetRenderAPIChange(Shader* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }