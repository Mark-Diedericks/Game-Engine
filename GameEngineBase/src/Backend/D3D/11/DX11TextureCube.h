#pragma once

#include "Backend/API/APITextureCube.h"
#include "DX11Common.h"

namespace gebase { namespace graphics { namespace API {

	class DX11TextureCube : public APITextureCube
	{
	private:
		String m_Name;
		String m_File;
		uint* m_Width;
		uint* m_Height;
		uint m_Bits;
		uint m_Mips;

		uint* m_FaceWidths;
		uint* m_FaceHeights;

		InputFormat m_Format;

		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ResourceView;
		ID3D11SamplerState* m_SamplerState;
		D3D11_SAMPLER_DESC m_SamplerDesc;
		
		uint LoadFromSingleFile(const byte* pixels, uint bits);
		uint LoadFromMultipleFiles(const byte** sides, uint bits);
		uint LoadFromVerticalCross(const byte** sides, uint* width, uint* height, uint bits, uint mips);
		uint LoadFromVerticalCross(const byte*** sides, uint* width, uint* height, uint bits, uint mips);
	public:
		DX11TextureCube(const String& name, const byte* pixels, uint width, uint height, uint bits);
		DX11TextureCube(const String& name, const byte** sides, uint width, uint height, uint bits);
		DX11TextureCube(const String& name, const byte** sides, int32 mips, uint* width, uint* height, uint bits, InputFormat format);
		DX11TextureCube(const String& name, const byte*** faces, int32 mips, uint* faceWidths, uint* faceHeights, uint bits, InputFormat format);
		~DX11TextureCube();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(byte*** pixels) override;
		uint getSize() const override;

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_File; }
		inline uint getWidth() const override { return m_Width[0]; }
		inline uint getHeight() const override { return m_Height[0]; }
	};

} } }