#pragma once

#include "Backend/API/APIVertexBuffer.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLVertexBuffer : public APIVertexBuffer
	{
	private:
		uint m_Handle;
		uint m_Size;
		BufferUsage m_Usage;
		APIBufferLayout m_Layout;
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
		void ReleasePointer() override;
	};

} } }