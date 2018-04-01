#pragma once

#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Mesh/Mesh.h"
#include "GLCommon.h"
#include "GLShader.h"

namespace gebase { namespace graphics { 
	class GLVertexBuffer : public VertexBuffer
	{
	private:
		uint m_Handle;

		BufferUsage m_Usage;
		BufferLayout m_Layout;
		uint m_Size;

		GLShader* m_Shader;
	protected:
		void* getPointerInternal() override;
	public:
		GLVertexBuffer(BufferUsage usage);
		~GLVertexBuffer();
		
		void Bind() override;
		void Unbind() override;

		void Resize(uint size) override;
		void setLayout(const BufferLayout& layout, const Shader* shader) override;

		void setData(uint size, byte* data, bool del = true) override;

		void getBufferData(byte* data) override;
		inline Shader* getShader() const override { return m_Shader; }
		inline uint getSize() override { return m_Size; }
		inline BufferLayout getBufferLayout() override { return m_Layout; }

		void ReleasePointer() override;
	};

} }