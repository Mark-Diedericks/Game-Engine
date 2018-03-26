#pragma once

#include "Graphics/Texture/TextureCube.h"
#include "GLCommon.h"

namespace gebase { namespace graphics {

	class GLTextureCube : public TextureCube
	{
	private:
		uint m_Handle;
		uint* m_Width;
		uint* m_Height;

		String m_Name;
		String m_File;

		uint m_BitsPerPixel;
		uint m_Mips;

		uint* m_FaceWidths;
		uint* m_FaceHeights;

		InputFormat m_Format;
		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;

		uint LoadFromSingleFile(const byte* pixels, uint bits);
		uint LoadFromMultipleFiles(const byte** sides, uint bits);
		uint LoadFromVerticalCross(const byte** miptextures, uint* width, uint* height, uint bits, uint mips);
		uint LoadFromVerticalCross(const byte*** faces, uint* width, uint* height, uint bits, uint mips);
	public:
		GLTextureCube(const String& name, const byte* pixels, uint width, uint height, uint bits);
		GLTextureCube(const String& name, const byte** sides, uint width, uint height, uint bits);
		GLTextureCube(const String& name, const byte** miptextures, int32 mips, uint* width, uint* height, uint bits, InputFormat format);
		GLTextureCube(const String& name, const byte*** faces, int32 mips, uint* faceWidths, uint* faceHeights, uint bits, InputFormat format);
		~GLTextureCube();

		void getPixelData(byte*** pixels) override;
		uint getSize() const override;

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_File; }

		inline uint getWidth() const override { return m_Width[0]; }
		inline uint getHeight() const override { return m_Height[0]; }

		inline uint getWidth(uint index) const override { return m_Width[index]; }
		inline uint getHeight(uint index) const override { return m_Height[index]; }

		inline uint getBitsPerPixel() const override { return m_BitsPerPixel; }
		inline uint getMips() const override { return m_Mips; }

		inline TextureParameters getParameters() const override { return m_Parameters; }
	};

} }