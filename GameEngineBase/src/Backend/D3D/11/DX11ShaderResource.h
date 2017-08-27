#pragma once

#include "Common.h"
#include "CustomString.h"

#include "DX11Common.h"
#include "Backend/API/APIShaderResource.h"

namespace gebase { namespace graphics { namespace API { 

	class DX11ShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	private:
		friend class DX11Shader;
	public:
		enum class ResourceType
		{
			NONE,
			TEXTURE2D,
			TEXTURECUBE,
			SAMPLERSTATE
		};
	private:
		ResourceType m_Type;
		String m_Name;
		uint m_Count;
		uint m_Register;
	public:
		DX11ShaderResourceDeclaration(ResourceType type, const String& name, uint count = 1);

		inline const String& getName() const override { return m_Name; }
		inline uint getRegister() const override { return m_Register; }

		inline ResourceType getType() const { return m_Type; }
		inline uint getCount() const { return m_Count; }

		static ResourceType StringToType(const String& type);
		static String TypeToString(ResourceType type);
	};

} } }