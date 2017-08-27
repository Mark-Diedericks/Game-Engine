#pragma once

#include "Common.h"
#include "String.h"
#include "Backend/API/APIShaderResource.h"
#include "GLShaderUniform.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	public:
		enum class ResourceType
		{
			NONE,
			TEXTURE2D,
			TEXTURECUBE,
			TEXTURESHADOW
		};
	private:
		friend class GLShader;
		
		String m_Name;
		uint m_Register;
		uint m_Count;
		ResourceType m_Type;
	public:
		GLShaderResourceDeclaration(ResourceType type, const String& name, uint count);

		inline const String& getName() const override { return m_Name; }
		inline uint getRegister() const override { return m_Register; }
		inline uint getCount() const override { return m_Count; }

		inline ResourceType getType() const { return m_Type; }

		static ResourceType StringToType(const String& type);
		static String TypeToString(ResourceType type);
	};

} } }