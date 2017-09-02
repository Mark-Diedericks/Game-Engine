#pragma once

#include "Backend/API/APIVertexBuffer.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLVertexBuffer : public APIVertexBuffer
	{
	private:
		uint m_Handle;

		BufferUsage m_Usage;
		APIBufferLayout m_Layout;
		uint m_Size;
	protected:
		void* getPointerInternal() override;
	public:
		GLVertexBuffer(BufferUsage usage);
		~GLVertexBuffer();
		
		void Bind() override;
		void Unbind() override;

		void Resize(uint size) override;
		void setLayout(const APIBufferLayout& layout) override;
		void setData(uint size, const void* data) override;

		void* getBufferData() override;
		inline uint getSize() override { return m_Size; }
		inline APIBufferLayout getBufferLayout() override { return m_Layout; }

		void ReleasePointer() override;
	};

} } }