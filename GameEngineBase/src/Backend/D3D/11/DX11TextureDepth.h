#pragma once

#include "Graphics/Texture/TextureDepth.h"

namespace gebase { namespace graphics {

	class DX11TextureDepth : public TextureDepth
	{
	private:
		String m_Name;

		uint m_Width;
		uint m_Height;
		uint m_Size;

		uint m_BitsPerPixel;
		uint m_Mips;
		
		TextureParameters m_Parameters;
	protected:
		void Init();
	public:
		DX11TextureDepth(uint width, uint height);
		~DX11TextureDepth();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(uint16* pixels) override;
		uint getSize() const override;
		void setData(const uint16* data) override;

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Name; }

		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }

		inline uint getBitsPerPixel() const override { return m_BitsPerPixel; }
		inline uint getMips() const override { return m_Mips; }

		inline TextureParameters getParameters() const override { return m_Parameters; }
	};

} }