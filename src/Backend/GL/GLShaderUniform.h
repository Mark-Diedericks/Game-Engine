#pragma once

#include "Common.h"
#include "Backend/API/APIShaderUniform.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLShaderUniformDeclaration : public ShaderUniformDeclaration
	{
	private:
		friend class GLShader;
		friend class GLShaderUniformBufferDeclaration;
	public:
		enum class UniformType
		{
			NONE,
			FLOAT32,
			VEC2,
			VEC3,
			VEC4,
			MAT3,
			MAT4,
			INT32,
			STRUCT
		};
	private:
		String m_Name;
		uint m_Size;
		uint m_Count;
		uint m_Offset;

		UniformType m_Type;
		ShaderStruct* m_Struct;
		mutable int32 m_Location;
	protected:
		void setOffset(uint offset) override;
	public:
		GLShaderUniformDeclaration(UniformType type, const String& name, uint count = 1);
		GLShaderUniformDeclaration(ShaderStruct* uniformStruct, const String& name, uint count = 1);

		inline const String& getName() const override { return m_Name; }
		inline uint getSize() const override { return m_Size; }
		inline uint getCount() const override { return m_Count; }
		inline uint getOffset() const override { return m_Offset; }
		inline uint getAbsoluteOffset() const { return m_Struct ? m_Struct->getOffset() + m_Offset : m_Offset; }

		int32 getLocation() const { return m_Location; }
		inline UniformType getType() const { return m_Type; }
		inline const ShaderStruct& getShaderUniformStruct() const { 
			if (m_Struct == nullptr) 
			{ 
				std::cout << "[GLShaderUniform] getShaderUniformStruct() - ShaderStruct is null." << std::endl; 
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}

			return *m_Struct; 
		}

		static uint SizeOfUniformType(UniformType type);
		static UniformType StringToType(const String& type);
		static String TypeToString(UniformType type);
	};

	struct GE_API GLShaderUniformField
	{
		GLShaderUniformDeclaration::UniformType type;
		String name;
		uint count;
		mutable uint size;
		mutable int32 location;
	};

	class GLShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
	{
	private:
		friend class Shader;

		String m_Name;
		ShaderUniformList m_Uniforms;
		uint m_Register;
		uint m_Size;
		uint m_ShaderType; //TODO: Enum - Vertex Shader (0), Fragment Shader (1)
	public:
		GLShaderUniformBufferDeclaration(const String& name, uint shaderType);

		void PushUniform(GLShaderUniformDeclaration* uniform);
		ShaderUniformDeclaration* FindUniform(const String& name);

		inline const String& getName() const override { return m_Name; }
		inline uint getRegister() const override { return m_Register; }
		inline uint getSize() const override { return m_Size; }
		inline uint getShaderType() const override { return m_ShaderType; }
		inline const ShaderUniformList& getUniformDeclarations() const override { return m_Uniforms; }
	};

} } }