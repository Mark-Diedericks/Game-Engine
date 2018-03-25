#pragma once

#include "Graphics/Texture/TextureDepth.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLTextureDepth : public TextureDepth
	{
	private:
		String m_Name;
		uint m_Handle;
		uint m_Width;
		uint m_Height;

		uint m_BitsPerPixel;
		uint m_Mips;

		TextureParameters m_Parameters;
	protected:
		uint Init();
	public:
		GLTextureDepth(uint width, uint height);
		~GLTextureDepth();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(uint16* pixels) override;
		uint getSize() const override; 
		void setData(const uint16* data) override;

		inline uint getHandle() const { return m_Handle; }
		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Name; }

		inline uint getBitsPerPixel() const override { return m_BitsPerPixel; }
		inline uint getMips() const override { return m_Mips; }

		inline TextureParameters getParameters() const override { return m_Parameters; }
	};

} }