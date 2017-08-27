#include "ge.h"
#include "DX11ShaderResource.h"

namespace gebase { namespace graphics { namespace API {

	DX11ShaderResourceDeclaration::DX11ShaderResourceDeclaration(ResourceType type, const String& name, uint count) : m_Type(type), m_Name(name), m_Count(count) { }

	DX11ShaderResourceDeclaration::ResourceType DX11ShaderResourceDeclaration::StringToType(const String& type)
	{
		if (type == "Texture2D") return ResourceType::TEXTURE2D;
		if (type == "TextureCube") return ResourceType::TEXTURECUBE;
		if (type == "SamplerState") return ResourceType::SAMPLERSTATE;	

		return ResourceType::NONE;
	}

	String DX11ShaderResourceDeclaration::TypeToString(ResourceType type)
	{
		switch (type)
		{
		case ResourceType::TEXTURE2D: return "Texture2D";
		case ResourceType::TEXTURECUBE: return "TextureCube";
		case ResourceType::SAMPLERSTATE: return "SamplerState";
		}

		return "";
	}

} } }