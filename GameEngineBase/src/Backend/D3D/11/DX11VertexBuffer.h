#pragma once

#include "Graphics\Buffer\VertexBuffer.h"
#include "DX11Common.h"

namespace gebase { namespace graphics {

	class DX11VertexBuffer : public VertexBuffer
	{
	private:
		D3D11_BUFFER_DESC m_BufferDesc;
		D3D11_MAPPED_SUBRESOURCE m_MappedSubresource;
		ID3D11Buffer* m_Handle;
		ID3D11InputLayout* m_InputLayout;

		BufferUsage m_Usage;
		BufferLayout m_Layout;
		uint m_Size;
	protected:
		void* getPointerInternal() override;
	public:
		DX11VertexBuffer(BufferUsage usage);
		~DX11VertexBuffer();

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