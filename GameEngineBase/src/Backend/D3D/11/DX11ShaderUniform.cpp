#include "ge.h"
#include "Common.h"

#include "DX11ShaderUniform.h"

namespace gebase { namespace graphics { namespace API {

#define BUFFER_ALIGNMENT 16

	DX11ShaderUniformDeclaration::DX11ShaderUniformDeclaration(UniformType type, const String& name, uint count) : m_Type(type), m_Count(count), m_Name(name), m_Offset(0)
	{
		m_Size = SizeOfUniformType(type) * count;
	}

	DX11ShaderUniformDeclaration::DX11ShaderUniformDeclaration(ShaderStruct* shaderStruct, const String& name, uint count) : m_Type(UniformType::NONE), m_Count(count), m_Name(name), m_Offset(0), m_Struct(shaderStruct)
	{
		m_Size = m_Struct->getSize();
	}

	DX11ShaderUniformDeclaration::UniformType DX11ShaderUniformDeclaration::StringToType(const String& type)
	{
		if (type == "float") return UniformType::FLOAT32;
		if (type == "int32") return UniformType::INT32;
		if (type == "float2") return UniformType::VEC2;
		if (type == "float3") return UniformType::VEC3;
		if (type == "float4") return UniformType::VEC4;
		if (type == "float3x3") return UniformType::MAT3;
		if (type == "float4x4") return UniformType::MAT4;

		return UniformType::NONE;
	}

	String DX11ShaderUniformDeclaration::TypeToString(DX11ShaderUniformDeclaration::UniformType type)
	{
		switch (type)
		{
		case UniformType::FLOAT32:	return "float";
		case UniformType::INT32:	return "int32";
		case UniformType::VEC2:		return "float2";
		case UniformType::VEC3:		return "float3";
		case UniformType::VEC4:		return "float4";
		case UniformType::MAT3:		return "float3x3";
		case UniformType::MAT4:		return "float4x4";
		}

		return "";
	}

	uint DX11ShaderUniformDeclaration::SizeOfUniformType(UniformType type)
	{
		switch (type)
		{
		case UniformType::FLOAT32:	return 4 * 1 * 1; //Bytes for a float multiplied by the count in two dimensions
		case UniformType::INT32:	return 4 * 1 * 1; //Bytes for a int32 multiplied by the count in two dimensions
		case UniformType::VEC2:		return 4 * 2 * 1; //Bytes for a float multiplied by the count in two dimensions
		case UniformType::VEC3:		return 4 * 3 * 1; //Bytes for a float multiplied by the count in two dimensions
		case UniformType::VEC4:		return 4 * 4 * 1; //Bytes for a float multiplied by the count in two dimensions
		case UniformType::MAT3:		return 4 * 3 * 3; //Bytes for a float multiplied by the count in two dimensions
		case UniformType::MAT4:		return 4 * 4 * 4; //Bytes for a float multiplied by the count in two dimensions
		}

		return 0;
	}

	DX11ShaderUniformBufferDeclaration::DX11ShaderUniformBufferDeclaration(const String& name, uint bufferRegister, uint shaderType) : m_Name(name), m_Register(bufferRegister), m_ShaderType(shaderType), m_Size(0) { }

	void DX11ShaderUniformBufferDeclaration::PushUniform(DX11ShaderUniformDeclaration* uniform)
	{
		uint offset = 0;

		if (m_Uniforms.size())
		{
			DX11ShaderUniformDeclaration* previous = (DX11ShaderUniformDeclaration*)m_Uniforms.back();
			offset = previous->m_Offset + previous->m_Size;
		}

		uniform->m_Offset = offset;
		m_Size += uniform->getSize();
		m_Uniforms.push_back(uniform);
	}

	void DX11ShaderUniformBufferDeclaration::Align()
	{
		m_Size = (m_Size + (BUFFER_ALIGNMENT - 1)) / BUFFER_ALIGNMENT * BUFFER_ALIGNMENT;
	}

	ShaderUniformDeclaration* DX11ShaderUniformBufferDeclaration::FindUniform(const String& name)
	{
		for (ShaderUniformDeclaration* uniform : m_Uniforms)
			if (uniform->getName() == name)
				return uniform;
		return nullptr;
	}

} } }