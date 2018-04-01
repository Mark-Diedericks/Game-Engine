#include "ge.h"
#include "Common.h"

#include "System\Memory.h"

#include "DX11IndexBuffer.h"
#include "DX11Context.h"

namespace gebase { namespace graphics {

	DX11IndexBuffer::DX11IndexBuffer(uint16* data, uint count) : IndexBuffer(0), m_Count(count)
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

		//if (data != nullptr)
			gedel[] data;
	}

	DX11IndexBuffer::DX11IndexBuffer(uint* data, uint count) : IndexBuffer(1), m_Count(count)
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

		if(data != nullptr)
			gedel[] data;
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

	void DX11IndexBuffer::getIndexData32(uint* data)
	{
		D3D11_MAPPED_SUBRESOURCE mapInfo;

		HRESULT hr;
		hr = DX11Context::getDeviceContext()->Map(m_Handle, 0, D3D11_MAP_READ, 0, &mapInfo);

		if (FAILED(hr))
		{
			if (hr == E_INVALIDARG)
			{
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

				desc.Usage = D3D11_USAGE_STAGING;
				desc.ByteWidth = m_Count * sizeof(uint);
				desc.BindFlags = 0;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				desc.MiscFlags = 0;

				ID3D11Buffer* stagingBuffer;
				hr = DX11Context::getDevice()->CreateBuffer(&desc, nullptr, &stagingBuffer);

				if (FAILED(hr))
				{
					utils::LogUtil::WriteLine("ERROR", "Could not map DX11Texture2D: " + std::to_string(hr));
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				DX11Context::getDeviceContext()->CopyResource(stagingBuffer, m_Handle);
				hr = DX11Context::getDeviceContext()->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mapInfo);

				if (FAILED(hr))
				{
					utils::LogUtil::WriteLine("ERROR", "Could not map DX11Texture2D: " + std::to_string(hr));
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				memcpy(data, mapInfo.pData, m_Count * sizeof(uint));
				DX11Context::getDeviceContext()->Unmap(stagingBuffer, 0);
				stagingBuffer->Release();
			}
			else
			{
				utils::LogUtil::WriteLine("ERROR", "Could not map DX11Texture2D: " + std::to_string(hr));
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}
		else
		{
			memcpy(data, mapInfo.pData, m_Count * sizeof(uint));
			DX11Context::getDeviceContext()->Unmap(m_Handle, 0);
		}
	}

	void DX11IndexBuffer::getIndexData16(uint16* data)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		D3D11_MAPPED_SUBRESOURCE mapInfo;

		HRESULT hr;
		hr = DX11Context::getDeviceContext()->Map(m_Handle, 0, D3D11_MAP_READ, 0, &mapInfo);

		if (FAILED(hr))
		{
			if (hr == E_INVALIDARG)
			{
				desc.Usage = D3D11_USAGE_STAGING;
				desc.ByteWidth = m_Count * sizeof(uint16);
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				desc.MiscFlags = 0;

				ID3D11Buffer* stagingBuffer;
				hr = DX11Context::getDevice()->CreateBuffer(&desc, nullptr, &stagingBuffer);

				if (FAILED(hr))
				{
					utils::LogUtil::WriteLine("ERROR", "Could not map DX11IndexBuffer: " + std::to_string(hr));
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				DX11Context::getDeviceContext()->CopyResource(stagingBuffer, m_Handle);
				hr = DX11Context::getDeviceContext()->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mapInfo);

				if (FAILED(hr))
				{
					utils::LogUtil::WriteLine("ERROR", "Could not map DX11IndexBuffer: " + std::to_string(hr));
#ifdef GE_DEBUG
					__debugbreak();
#endif
				}

				memcpy(data, (byte*)mapInfo.pData, m_Count * sizeof(uint16));
				DX11Context::getDeviceContext()->Unmap(stagingBuffer, 0);
				stagingBuffer->Release();
			}
			else
			{
				utils::LogUtil::WriteLine("ERROR", "Could not map DX11IndexBuffer: " + std::to_string(hr));
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}
		else
		{
			memcpy(data, mapInfo.pData, m_Count * sizeof(uint16));
			DX11Context::getDeviceContext()->Unmap(m_Handle, 0);
		}
	}

} }