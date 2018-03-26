#pragma once

#include "Graphics/Texture/Texture2D.h"
#include "DX11Common.h"

namespace gebase { namespace graphics {

	class DX11Texture2D : public Texture2D
	{
	private:
		String m_Name;
		String m_Filepath;
		uint m_Handle;
		uint m_Width;
		uint m_Height;

		uint m_BitsPerPixel;
		uint m_Mips;

		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;

		D3D11_TEXTURE2D_DESC m_TextureDesc;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ResourceView;
		ID3D11SamplerState* m_SamplerState;
		D3D11_SAMPLER_DESC m_SamplerDesc;

		void Load(const byte* pixels, uint bits);
	public:
		DX11Texture2D(uint width, uint height, TextureParameters parameters = TextureParameters());
		DX11Texture2D(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters = TextureParameters());
		~DX11Texture2D();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(byte* pixels) override;
		uint getSize() const override;

		void setData(const byte* pixels, const bool del = true) override;
		void setData(uint color) override;

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }

		inline const String& getName() const { return m_Name; }
		inline const String& getFilepath() const { return m_Filepath; }

		inline uint getBitsPerPixel() const override { return m_BitsPerPixel; }
		inline uint getMips() const override { return m_Mips; }

		inline TextureParameters getParameters() const override { return m_Parameters; }
	};

} }