#pragma once

#include "Pch.h"
#include "Common.h"
#include "Backend/API/APIBufferLayout.h"
#include "Backend/API/APIVertexBuffer.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {
	
	class VertexBuffer
	{
	private:
		const void* m_Data;
		API::APIVertexBuffer* m_Instance;
		VertexBuffer();
	public:
		inline void Resize(uint size) { m_Instance->Resize(size); }
		inline void setLayout(const API::APIBufferLayout& layout) { m_Instance->setLayout(layout); }
		void setData(uint size, const void* data);

		inline void ReleasePointer() { m_Instance->ReleasePointer(); }

		inline void Bind() { m_Instance->Bind(); }
		inline void Unbind() { m_Instance->Unbind(); }

		inline API::APIVertexBuffer* getInstance() const { return m_Instance; }

		template <typename T>
		T* getPointer()
		{
			return (T*)m_Instance->getPointer();
		}

		static VertexBuffer* Create(API::BufferUsage = API::BufferUsage::STATIC);
	};

} }