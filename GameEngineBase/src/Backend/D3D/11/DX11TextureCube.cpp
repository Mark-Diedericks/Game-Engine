#include "ge.h"
#include "Common.h"
#include "DX11TextureCube.h"
#include "DX11Context.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	DX11TextureCube::DX11TextureCube(const String& name, const byte* pixels, uint width, uint height, uint bits) : m_Name(name), m_File(name), m_Width(width), m_Height(height), m_Bits(bits)
	{
		LoadFromSingleFile(pixels, bits);
	}

	DX11TextureCube::DX11TextureCube(const String& name, const byte** sides, uint width, uint height, uint bits) : m_Name(name), m_File(name), m_Width(width), m_Height(height), m_Bits(bits)
	{
		LoadFromMultipleFiles(sides, bits);
	}

	DX11TextureCube::DX11TextureCube(const String& name, const byte** sides, int32 mips, uint* width, uint* height, uint bits, InputFormat format) : m_Name(name), m_File(name), m_Width(width[0]), m_Height(height[0]), m_Bits(bits), m_Format(format)
	{
		if (format == InputFormat::VERTICAL_CROSS)
			LoadFromVerticalCross(sides, width, height, bits, mips);
	}

	DX11TextureCube::~DX11TextureCube()
	{

	}

	uint DX11TextureCube::LoadFromSingleFile(const byte* pixels, uint mbits)
	{
		//TODO: Implement
		return 0;
	}

	uint DX11TextureCube::LoadFromMultipleFiles(const byte** sides, uint mbits)
	{
		//TODO: Implement
		return 0;
	}

	uint DX11TextureCube::LoadFromVerticalCross(const byte** sides, uint* width, uint* height, uint mbits, uint mips)
	{
		uint srcWidth = m_Width;
		uint srcHeight = m_Height;
		uint bits = mbits;

		byte*** cubeTextureData = genew byte**[mips];

		for (uint i = 0; i < mips; i++)
			cubeTextureData[i] = genew byte*[6];

		uint* fWidths = genew uint[mips];
		uint* fHeights = genew uint[mips];

		for (uint m = 0; m < mips; m++)
		{
			const byte* data = sides[m];
			srcWidth = width[m];
			srcHeight = height[m];

			uint stride = bits / 8;

			uint face = 0;
			uint fWidth = srcWidth / 3;
			uint fHeight = srcHeight / 4;

			fWidths[m] = fWidth;
			fHeights[m] = fHeight;

			for (uint cy = 0; cy < 4; cy++)
			{
				for (uint cx = 0; cx < 3; cx++)
				{
					if (cy == 0 || cy == 2 || cy == 3)
						if (cx != 1)
							continue;

					cubeTextureData[m][face] = genew byte[fWidth * fHeight * stride];
					uint index = 0;

					for (uint y = 0; y < fHeight; y++)
					{
						uint offset = y;
						if (face == 5)
							offset = fHeight - (y + 1);

						uint yp = cy * fHeight + offset;

						for (uint x = 0; x < fWidth; x++)
						{
							offset = x;
							if (face == 5)
								offset = fWidth - (x + 1);

							uint xp = cx * fHeight + offset;
							uint xyws = (x + y * fWidth) * stride;
							uint sxyws = (xp + yp * srcWidth) * stride;

											 cubeTextureData[m][face][xyws + 0] = data[sxyws + 0];
											 cubeTextureData[m][face][xyws + 1] = data[sxyws + 1];
											 cubeTextureData[m][face][xyws + 2] = data[sxyws + 2];
							if (stride >= 4) cubeTextureData[m][face][xyws + 3] = data[sxyws + 3];
						}
					}

					face++;
				}
			}

			gedel[] data;
		}

		D3D11_TEXTURE2D_DESC td;
		td.Width = fWidths[0];
		td.Height = fHeights[0];
		td.MipLevels = mips;
		td.ArraySize = 6;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.CPUAccessFlags = 0;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC srd;
		srd.Format = td.Format;
		srd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srd.TextureCube.MipLevels = td.MipLevels;
		srd.TextureCube.MostDetailedMip = 0;

		D3D11_SUBRESOURCE_DATA* pData = genew D3D11_SUBRESOURCE_DATA[6 * mips];

		uint result = 0;
		uint index = 0;
		uint faceOrder[6] = {3, 1, 0, 4, 2, 5};

		for (int32 f = 0; f < 6; f++)
		{
			uint fi = faceOrder[f];
			for (int32 m = 0; m < (int32)mips; m++)
			{
				pData[index].pSysMem = cubeTextureData[m][fi];
				pData[index].SysMemPitch = fWidths[m] * 4;
				pData[index].SysMemSlicePitch = fWidths[m] * fHeights[m] * 4;
				index++;
			}
		}

		m_Texture = nullptr;

		DXCall(DX11Context::getDevice()->CreateTexture2D(&td, pData, &m_Texture));
		DXCall(DX11Context::getDevice()->CreateShaderResourceView(m_Texture, &srd, &m_ResourceView));

		gedel[] pData;

		for (int32 m = 0; m < (int32)mips; m++)
		{
			for (int32 f = 0; f < 6; f++)
				gedel[] cubeTextureData[m][f];
			gedel[] cubeTextureData[m];
		}
		gedel[] cubeTextureData;

		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXCall(DX11Context::getDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState));

		return 0;
	}

	void DX11TextureCube::Bind(uint slot) const
	{
		DX11Context::getDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		DX11Context::getDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void DX11TextureCube::Unbind(uint slot) const
	{
		ID3D11ShaderResourceView* nullView = nullptr;
		DX11Context::getDeviceContext()->PSSetShaderResources(slot, 1, &nullView);
	}

} } }