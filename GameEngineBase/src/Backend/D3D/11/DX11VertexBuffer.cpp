#include "ge.h"
#include "Common.h"
#include "DX11VertexBuffer.h"

#include "DX11Context.h"
#include "DX11Convert.h"
#include "DX11Shader.h"

#include "Backend/API/APIConvert.h"

namespace gebase { namespace graphics {

	DX11VertexBuffer::DX11VertexBuffer(BufferUsage usage) : VertexBuffer(0), m_Usage(usage), m_Size(0)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = (D3D11_USAGE)DX11Convert::BufferUsageToDX(usage);
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{

	}

	void DX11VertexBuffer::Resize(uint size)
	{
		m_Size = size;
		m_BufferDesc.ByteWidth = size;
		DXCall(DX11Context::getDevice()->CreateBuffer(&m_BufferDesc, NULL, &m_Handle));
	}

	void DX11VertexBuffer::setLayout(const BufferLayout& bufferLayout, const Shader* shader)
	{
		m_Layout = bufferLayout;
		const std::vector<BufferElement>& layout = bufferLayout.getLayout();
		D3D11_INPUT_ELEMENT_DESC* desc = genew D3D11_INPUT_ELEMENT_DESC[layout.size()];

		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			desc[i] = { element.name.c_str(), 0, (DXGI_FORMAT)APIConvert::BufferElementTypeToDX(element.type), 0, element.offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		//const DX11Shader* shader = DX11Shader::CurrentlyBound();
		if (shader != nullptr)
			m_Shader = (DX11Shader*)shader;

		if (!m_Shader)
		{
			utils::LogUtil::WriteLine("ERROR", "[DX11VertexBuffer] setLayout() - Shader assertion failed");
#ifdef GE_DEBUG
			__debugbreak();
#endif
			return;
		}

		DXCall(DX11Context::getDevice()->CreateInputLayout(desc, layout.size(), m_Shader->getData().vs->GetBufferPointer(), m_Shader->getData().vs->GetBufferSize(), &m_InputLayout));
		gedel desc;
	}

	void DX11VertexBuffer::setData(uint size, byte* data)
	{
		if (m_Size < size)
			Resize(size);

		getPointerInternal();
		memcpy(m_MappedSubresource.pData, data, size);
		ReleasePointer();

		if (data != nullptr)
			gedel[] data;
	}

	void DX11VertexBuffer::setData(uint size, Vertex* data)
	{
		if (m_Size < size)
			Resize(size);

		getPointerInternal();
		memcpy(m_MappedSubresource.pData, data, size);
		ReleasePointer();
	}

	void DX11VertexBuffer::setData(uint size, QuadVertex* data)
	{
		if (m_Size < size)
			Resize(size);

		getPointerInternal();
		memcpy(m_MappedSubresource.pData, data, size);
		ReleasePointer();
	}

	void DX11VertexBuffer::Bind()
	{
		uint offset = 0;
		uint stride = m_Layout.getStride();
		DX11Context::getDeviceContext()->IASetInputLayout(m_InputLayout);
		DX11Context::getDeviceContext()->IASetVertexBuffers(0, 1, &m_Handle, &stride, &offset);
	}

	void DX11VertexBuffer::Unbind()
	{

	}

	void* DX11VertexBuffer::getPointerInternal()
	{
		DXCall(DX11Context::getDeviceContext()->Map(m_Handle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresource));
		return m_MappedSubresource.pData;
	}

	void DX11VertexBuffer::ReleasePointer()
	{
		DX11Context::getDeviceContext()->Unmap(m_Handle, NULL);
	}

	void DX11VertexBuffer::getBufferData(void* data)
	{
		data = nullptr;
	}

} }