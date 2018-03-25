#pragma once

#include "Graphics\Buffer\VertexBuffer.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { 

	class GLVertexBuffer : public VertexBuffer
	{
	private:
		uint m_Handle;

		BufferUsage m_Usage;
		BufferLayout m_Layout;
		uint m_Size;
	protected:
		void* getPointerInternal() override;
	public:
		GLVertexBuffer(BufferUsage usage);
		~GLVertexBuffer();
		
		void Bind() override;
		void Unbind() override;

		void Resize(uint size) override;
		void setLayout(const BufferLayout& layout) override;
		void setData(uint size, const void* data) override;

		void getBufferData(void* data) override;
		inline uint getSize() override { return m_Size; }
		inline BufferLayout getBufferLayout() override { return m_Layout; }

		void ReleasePointer() override;
	};

} }