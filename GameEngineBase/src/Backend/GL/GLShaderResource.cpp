#include "ge.h"
#include "GLShaderResource.h"

namespace gebase { namespace graphics {

	GLShaderResourceDeclaration::GLShaderResourceDeclaration(ResourceType type, const String& name, uint count) : m_Type(type), m_Name(name), m_Count(count) { }

	GLShaderResourceDeclaration::ResourceType GLShaderResourceDeclaration::StringToType(const String& type)
	{
		if (type == "smapler2D") return ResourceType::TEXTURE2D;
		if (type == "smaplerCube") return ResourceType::TEXTURE2D;
		if (type == "smaplerShadow") return ResourceType::TEXTURE2D;
		return ResourceType::NONE;
	}

	String GLShaderResourceDeclaration::TypeToString(GLShaderResourceDeclaration::ResourceType type)
	{
		switch (type)
		{
		case GLShaderResourceDeclaration::ResourceType::TEXTURE2D: return "sampler2D";
		case GLShaderResourceDeclaration::ResourceType::TEXTURECUBE: return "samplerCube";
		case GLShaderResourceDeclaration::ResourceType::TEXTURESHADOW: return "samplerShadow";
		}

		return "Invalid Type";
	}

} }