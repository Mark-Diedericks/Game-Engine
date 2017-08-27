#pragma once

#include "Math/Maths.h"
#include "Backend/API/APIShader.h"
#include "Graphics/Shader/ShaderManager.h"
#include "GLCommon.h"
#include "GLShaderUniform.h"
#include "GLShaderResource.h"

namespace gebase { namespace graphics { namespace API {

	struct GLShaderErrorInfo
	{
		uint shader;
		String message[2];
		uint line[2];
	};

	class GLShader : public APIShader
	{
	private:
		friend class APIShader;
		friend class Shader;
		friend class ShaderManager;

		uint m_Handle;
		String m_Name;
		String m_Path;
		String m_Source;
		String m_VertexSource;
		String m_FragmentSource;

		ShaderUniformBufferList m_VSUniformBuffers;
		ShaderUniformBufferList m_FSUniformBuffers;

		GLShaderUniformBufferDeclaration* m_VSUserUniformBuffer;
		GLShaderUniformBufferDeclaration* m_FSUserUniformBuffer;

		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;

		static uint Compile(String** sgaders, GLShaderErrorInfo& info = GLShaderErrorInfo());
		static void Preprocess(const String& source, String** shaders);

		void Parse(const String& vs, const String& fs);
		void ParseUniform(const String& statement, uint shaderType);
		void ParseUniformStruct(const String& block, uint shaderType);

		bool isTypeStringResource(const String& type);
		bool isSystemUniform(ShaderUniformDeclaration* uniform) const;

		void ResolveUniforms();
		void ValidateUniforms();

		int32 getUniformLocation(const String& name);
		ShaderStruct* FindStruct(const String& name);
		ShaderUniformDeclaration* FindUniformDeclaration(const String& name);
		ShaderUniformDeclaration* FindUniformDeclaration(const String& name, const ShaderUniformBufferDeclaration* buffer);

		void ResolveAndSetUniform(GLShaderUniformDeclaration* uniform, byte* data, uint size);
		void ResolveAndSetUniforms(ShaderUniformBufferDeclaration* buffer, byte* data, uint size);

		void setUniformStruct(GLShaderUniformDeclaration* uniform, byte* data, int32 offset);

		void setUniform1f(const String& name, float value);
		void setUniform1fv(const String& name, float* value, int32 count);
		void setUniform1i(const String& name, int32 value);
		void setUniform1iv(const String& name, int32* value, int32 count);
		void setUniform2f(const String& name, math::Vector2f& value);
		void setUniform3f(const String& name, math::Vector3f& value);
		void setUniform4f(const String& name, math::Vector4f& value);
		void setUniformMat4(const String& name, math::Matrix4f& value);


		void setUniform1f(uint location, float value);
		void setUniform1fv(uint location, float* value, int32 count);
		void setUniform1i(uint location, int32 value);
		void setUniform1iv(uint location, int32* value, int32 count);
		void setUniform2f(uint location, math::Vector2f& value);
		void setUniform3f(uint location, math::Vector3f& value);
		void setUniform4f(uint location, math::Vector4f& value);
		void setUniformMat4(uint location, math::Matrix4f& value);
	public:
		GLShader(const String& name, const String& source);
		~GLShader();

		void Init();
		void Shutdown();

		void Bind() const override;
		void Unbind() const override;

		void setVSSystemUniformBuffer(byte* data, uint size, uint slot) override;
		void setFSSystemUniformBuffer(byte* data, uint size, uint slot) override;

		void setVSUserUniformBuffer(byte* data, uint size) override;
		void setFSUserUniformBuffer(byte* data, uint size) override;

		void setUniform(const String& name, byte* data);
		void ResolveAndSetUniformField(const GLShaderUniformDeclaration& field, byte* data, int32 offset);

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Path; }

		inline const ShaderUniformBufferList& getVSSystemUniforms() const override { return m_VSUniformBuffers; }
		inline const ShaderUniformBufferList& getFSSystemUniforms() const override { return m_FSUniformBuffers; }

		inline const ShaderUniformBufferDeclaration* getVSUserUniformBuffer() const override { return m_VSUserUniformBuffer; }
		inline const ShaderUniformBufferDeclaration* getFSUserUniformBuffer() const override { return m_FSUserUniformBuffer; }
		
		inline const ShaderResourceList& getResources() const override { return m_Resources; }

		static bool TryCompile(const String& source, String& error);
	};

} } }