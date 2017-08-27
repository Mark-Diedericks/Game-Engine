#pragma once

#include "Common.h"
#include "Backend/API/APIIndexBuffer.h"

namespace gebase { namespace graphics {

	class GE_API IndexBuffer
	{
	private:
		uint16* m_Data16;
		uint* m_Data;

		API::APIIndexBuffer* m_Instance;
		IndexBuffer() { }
	public:
		inline uint getCount() const { return m_Instance->getCount(); }

		inline void Bind() const { m_Instance->Bind(); }
		inline void Unbind() const { m_Instance->Unbind(); }

		static IndexBuffer* Create(uint16* data, uint count);
		static IndexBuffer* Create(uint* data, uint count);
	};

} }