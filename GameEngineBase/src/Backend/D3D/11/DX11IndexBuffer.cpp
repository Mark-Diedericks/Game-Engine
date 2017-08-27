#include "ge.h"
#include "Common.h"
#include "DX11IndexBuffer.h"

#include "DX11Context.h"

namespace gebase { namespace graphics { namespace API {

	DX11IndexBuffer::DX11IndexBuffer(uint16* data, uint count) : m_Count(count)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint16);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibid;
		ibid.pSysMem = data;

		HRESULT hr = DX11Context::getDevice()->CreateBuffer(&ibd, &ibid, &m_Handle);
	}

	DX11IndexBuffer::DX11IndexBuffer(uint* data, uint count) : m_Count(count)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibid;
		ibid.pSysMem = data;

		HRESULT hr = DX11Context::getDevice()->CreateBuffer(&ibd, &ibid, &m_Handle);
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{

	}

	void DX11IndexBuffer::Bind() const
	{
		DX11Context::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Context::getDeviceContext()->IASetIndexBuffer(m_Handle, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind() const
	{

	}

} } }