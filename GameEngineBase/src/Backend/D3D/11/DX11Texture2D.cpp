#include "ge.h"
#include "Common.h"
#include "DX11Texture2D.h"

#include "DX11Common.h"
#include "DX11Context.h"
#include "DX11Convert.h"
#include "DX11TextureHandle.h"

#include "Utils/ImageUtil.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	DX11Texture2D::DX11Texture2D(uint width, uint height, TextureParameters parameters) : m_Filepath("NULL"), m_Width(width), m_Height(height), m_Parameters(parameters), m_BitsPerPixel(32)
	{
		Load(NULL, NULL);
	}

	DX11Texture2D::DX11Texture2D(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters) : m_Filepath(name), m_Width(width), m_Height(height), m_Parameters(parameters), m_BitsPerPixel(bits)
	{
		Load(pixels, bits);
	}

	DX11Texture2D::~DX11Texture2D()
	{
		
	}

	void DX11Texture2D::Load(const byte* data, uint bits)
	{
		bool generateMips = data != nullptr;
		uint stride = 4;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data;
		sd.SysMemPitch = stride * m_Width;
		sd.SysMemSlicePitch = stride * m_Width * m_Height;

		D3D11_SUBRESOURCE_DATA* sdPtr = nullptr;
		uint mipLvls = 1;

		if (generateMips)
		{
			uint width = m_Height;
			uint height = m_Height;

			while (width > 1 && height > 1)
			{
				width = max(width / 2, 1u);
				height = max(height / 2, 1u);
				++mipLvls;
			}
		}
		else
		{
			if (data)
				sdPtr = &sd;
		}

		DXGI_FORMAT format = DX11Convert::TextureFormatToDX(m_Parameters.format);

		uint formatSupport = 0;
		DX11Context::getDevice()->CheckFormatSupport(format, &formatSupport);

		if (!(formatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
		{
			utils::LogUtil::WriteLine("ERROR", "[DX11Texture2D] Load() - Format not supported");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		ZeroMemory(&m_TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		m_TextureDesc.Width = m_Width;
		m_TextureDesc.Height = m_Height;
		m_TextureDesc.MipLevels = mipLvls;
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.Format = format;
		m_TextureDesc.CPUAccessFlags = 0;
		m_TextureDesc.Usage = generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		m_TextureDesc.CPUAccessFlags = m_TextureDesc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE: 0;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (generateMips) m_TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_TextureDesc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
		m_TextureDesc.SampleDesc.Count = 1;
		m_TextureDesc.SampleDesc.Quality = 0;

		DXCall(DX11Context::getDevice()->CreateTexture2D(&m_TextureDesc, sdPtr, &m_Texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvd.Format = m_TextureDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = m_TextureDesc.MipLevels;

		DXCall(DX11Context::getDevice()->CreateShaderResourceView(m_Texture, &srvd, &m_ResourceView));
		if (generateMips)
		{
			DX11Context::getDeviceContext()->UpdateSubresource(m_Texture, 0, nullptr, sd.pSysMem, sd.SysMemPitch, sd.SysMemSlicePitch);
			DX11Context::getDeviceContext()->GenerateMips(m_ResourceView);
		}

		m_TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		m_TextureDesc.CPUAccessFlags = 0;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.MinLOD = 0;
		m_SamplerDesc.MaxLOD = 11;
		m_SamplerDesc.Filter = m_Parameters.filter == TextureFilter::LINEAR ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXCall(DX11Context::getDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState));

		if (data != nullptr)
			gedel[] data;
	}

	void DX11Texture2D::Bind(uint slot) const
	{
		DX11Context::getDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		DX11Context::getDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void DX11Texture2D::Unbind(uint slot) const
	{
		ID3D11ShaderResourceView* srv = nullptr;
		DX11Context::getDeviceContext()->PSSetShaderResources(slot, 1, &srv);
	}

	void DX11Texture2D::setData(const byte* pixels)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DXCall(DX11Context::getDeviceContext()->Map(m_Texture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr));

		for (uint i = 0; i < m_Width * m_Height * getStrideFromFormat(m_Parameters.format); i += 4)
		{
			((byte*)msr.pData)[i + 0] = 0xff;
			((byte*)msr.pData)[i + 1] = 0xff;
			((byte*)msr.pData)[i + 2] = 0xff;
			((byte*)msr.pData)[i + 3] = ((byte*)pixels)[i / 2 + 1];
		}

		DX11Context::getDeviceContext()->Unmap(m_Texture, NULL);
	}

	void DX11Texture2D::setData(uint color)
	{
		//TODO
	}

	void DX11Texture2D::getPixelData(byte* data)
	{
		data = nullptr;
	}

	uint DX11Texture2D::getSize() const
	{
		return 0;
	}

} } }