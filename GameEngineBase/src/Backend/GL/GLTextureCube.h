#pragma once

#include "Backend/API/APITextureCube.h"
#include "GLCommon.h"

namespace gebase { namespace graphics { namespace API {

	class GLTextureCube : public APITextureCube
	{
	private:
		uint m_Handle;
		uint m_Width;
		uint m_Height;

		String m_Name;
		String* m_Files;

		uint m_Bits;

		InputFormat m_Format;
		TextureParameters m_Parameters;
		TextureLoadOptions m_LoadOptions;

		uint LoadFromSingleFile(const byte* pixels, uint bits);
		uint LoadFromMultipleFiles(const byte** sides, uint bits);
		uint LoadFromVerticalCross(const byte** sides, uint bits, uint mips);
	public:
		GLTextureCube(const String& name, const byte* pixels, uint width, uint height, uint bits);
		GLTextureCube(const String& name, const byte** sides, uint width, uint height, uint bits);
		GLTextureCube(const String& name, const byte** sides, int32 mips, uint width, uint height, uint bits, InputFormat format);
		~GLTextureCube();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		inline const String& getName() const override { return m_Name; }
		inline const String& getFilepath() const override { return m_Files[0]; }
		inline uint getWidth() const override { return m_Width; }
		inline uint getHeight() const override { return m_Height; }
	};

} } }