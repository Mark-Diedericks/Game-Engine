#pragma once

#include "Backend/API/APIIndexBuffer.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLIndexBuffer : public APIIndexBuffer
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

		uint getCount() const { return m_Count; }
	};

} } }