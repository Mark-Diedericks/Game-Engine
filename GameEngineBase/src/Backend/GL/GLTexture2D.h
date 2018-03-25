#pragma once

#include "Graphics/Texture/Texture2D.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLTexture2D : public Texture2D
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

		uint Load(const byte* pixels, uint bits);
	public:
		GLTexture2D(uint width, uint height, TextureParameters parameters = TextureParameters());
		GLTexture2D(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters = TextureParameters());
		~GLTexture2D();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void getPixelData(byte* pixels) override;
		uint getSize() const override;

		void setData(const byte* pixels) override;
		void setData(uint color) override;

		uint getHandle() const { return m_Handle; }
		uint getWidth() const override { return m_Width; }
		uint getHeight() const override { return m_Height; }

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Filepath; }

		inline uint getBitsPerPixel() const override { return m_BitsPerPixel; }
		inline uint getMips() const override { return m_Mips; }

		inline TextureParameters getParameters() const override { return m_Parameters; }
	};

} }
