#pragma once

#include "Common.h"
#include "String.h"

namespace gebase { namespace graphics { namespace API {

	class ShaderUniformDeclaration
	{
	private:
		friend class Shader;
		friend class ShaderStruct;
		friend class GLShader;
		friend class VKShader;
		friend class DX11Shader;
		friend class DX12Shader;
	protected:
		virtual void setOffset(uint offset) = 0;
	public:
		virtual const String& getName() const = 0;
		virtual uint getSize() const = 0;
		virtual uint getCount() const = 0;
		virtual uint getOffset() const = 0;
	};

	typedef std::vector<ShaderUniformDeclaration*> ShaderUniformList;

	class ShaderUniformBufferDeclaration
	{
	public:
		virtual const String& getName() const = 0;
		virtual uint getRegister() const = 0;
		virtual uint getShaderType() const = 0;
		virtual uint getSize() const = 0;

		virtual const ShaderUniformList& getUniformDeclarations() const = 0;
		virtual ShaderUniformDeclaration* FindUniform(const String& name) = 0;
	};

	typedef std::vector<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;

	class ShaderStruct
	{
	private:
		friend class Shader;

		String m_Name;
		std::vector<ShaderUniformDeclaration*> m_Fields;
		uint m_Size;
		uint m_Offset;
	public:
		ShaderStruct(const String& name) : m_Name(name), m_Size(0), m_Offset(0) {}

		void AddField(ShaderUniformDeclaration* field)
		{
			m_Size += field->getSize();
			uint offset = 0;

			if (m_Fields.size())
			{
				ShaderUniformDeclaration* previous = m_Fields.back();
				offset = previous->getOffset() + previous->getSize();
			}

			field->setOffset(offset);
			m_Fields.push_back(field);
		}

		inline const std::vector<ShaderUniformDeclaration*>& getFields() const { return m_Fields; }

		inline void setOffset(uint offset) { m_Offset = offset; }

		inline const String& getName() const { return m_Name; }
		inline uint getSize() const { return m_Size; }
		inline uint getOffset() const { return m_Offset; }
	};

	typedef std::vector<ShaderStruct*> ShaderStructList;

} } }