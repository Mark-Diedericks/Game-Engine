#pragma once

#include "ge.h"
#include "Common.h"
#include "CustomString.h"

#include "DX11Common.h"
#include "Graphics/Shader/ShaderUniform.h"

namespace gebase { namespace graphics {

	class DX11ShaderUniformDeclaration : public ShaderUniformDeclaration
	{
	private:
		friend class DX11ShaderUniformBufferDeclaration;
		friend class DX11Shader;
		friend class ShaderUniformBufferDeclaration;
	public:
		enum class UniformType
		{
			NONE,
			FLOAT32,
			INT32,
			VEC2,
			VEC3,
			VEC4,
			MAT3,
			MAT4
		};
	private:
		String m_Name;
		uint m_Size;
		uint m_Count;
		uint m_Offset;
		UniformType m_Type;
		uint m_Register;

		ShaderStruct* m_Struct;
	protected:
		inline void setOffset(uint offset) override { m_Offset = offset; }
	public:
		DX11ShaderUniformDeclaration(UniformType type, const String& name, uint count = 1);
		DX11ShaderUniformDeclaration(ShaderStruct* shaderStruct, const String& name, uint count = 1);

		inline const String& getName() const override { return m_Name; }
		inline uint getSize() const override { return m_Size; }
		inline uint getCount() const override { return m_Count; }
		inline uint getOffset() const override { return m_Offset; }

		inline UniformType getType() const { return m_Type; }
		inline uint getRegister() const { return m_Register; }
		inline const ShaderStruct* getStruct() const { return m_Struct; }

		static UniformType StringToType(const String& type);
		static String TypeToString(UniformType type);
		static uint SizeOfUniformType(UniformType type);
	};

	class DX11ShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
	{
	private:
		friend class Shader;

		String m_Name;
		ShaderUniformList m_Uniforms;
		uint m_Register;
		uint m_Size;
		uint m_ShaderType;
	public:
		DX11ShaderUniformBufferDeclaration(const String& name, uint bufferRegister, uint shaderType);

		void PushUniform(DX11ShaderUniformDeclaration* uniform);
		void Align();

		ShaderUniformDeclaration* FindUniform(const String& name);

		inline const String& getName() const override { return m_Name; }
		inline uint getRegister() const override { return m_Register; }
		inline uint getShaderType() const override { return m_ShaderType; }
		inline uint getSize() const override { return m_Size; }
		inline const ShaderUniformList& getUniformDeclarations() const override { return m_Uniforms; }
	};

} }