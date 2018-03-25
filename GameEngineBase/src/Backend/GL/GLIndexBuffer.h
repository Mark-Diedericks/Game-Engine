#pragma once

#include "Graphics\Buffer\IndexBuffer.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLIndexBuffer : public IndexBuffer
	{
	private:
		uint m_Handle;
		uint m_Count;
	public:
		GLIndexBuffer(uint16* data, uint count);
		GLIndexBuffer(uint* data, uint count);
		~GLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		void getIndexData32(uint* data) override;
		void getIndexData16(uint16* data) override;

		uint getCount() const { return m_Count; }
	};

} }