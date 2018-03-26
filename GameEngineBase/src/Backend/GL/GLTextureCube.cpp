#include "ge.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLTextureCube.h"

#include <GL/glew.h>
#include <FreeImage.h>

#include "Utils/ImageUtil.h"
#include "System/Memory.h"
#include "Utils/LogUtil.h"

#include "GLTexture2D.h"

namespace gebase { namespace graphics {

	GLTextureCube::GLTextureCube(const String& name, const byte* pixels, uint width, uint height, uint bits) : TextureCube(0), m_Name(name), m_BitsPerPixel(bits)
	{
		m_File = name;
		m_Width[0] = width;
		m_Height[0] = height;

		m_Handle = LoadFromSingleFile(pixels, bits);
	}

	GLTextureCube::GLTextureCube(const String& name, const byte** sides, uint width, uint height, uint bits) : TextureCube(1), m_Name(name), m_BitsPerPixel(bits)
	{
		m_File = name;
		m_Width[0] = width;
		m_Height[0] = height;

		m_Handle = LoadFromMultipleFiles(sides, bits);
	}

	GLTextureCube::GLTextureCube(const String& name, const byte** miptextures, int32 mips, uint* width, uint* height, uint bits, InputFormat format) : TextureCube(2), m_Name(name), m_Width(width), m_Height(height), m_BitsPerPixel(bits)
	{
		m_File = name;

		if (format == InputFormat::VERTICAL_CROSS)
			m_Handle = LoadFromVerticalCross(miptextures, width, height, bits, mips);
	}

	GLTextureCube::GLTextureCube(const String& name, const byte*** faces, int32 mips, uint* faceWidths, uint* faceHeights, uint bits, InputFormat format) : TextureCube(2), m_Name(name), m_Width(faceWidths), m_Height(faceHeights), m_BitsPerPixel(bits)
	{
		m_File = name;

		if (format == InputFormat::VERTICAL_CROSS)
			m_Handle = LoadFromVerticalCross(faces, faceWidths, faceHeights, bits, mips);
	}

	GLTextureCube::~GLTextureCube()
	{
		GLCall(glDeleteTextures(1, &m_Handle));
		gedel[] m_Width;
		gedel[] m_Height;
		gedel[] m_FaceWidths;
		gedel[] m_FaceHeights;
	}


	void GLTextureCube::Bind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle));
	}

	void GLTextureCube::Unbind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	uint GLTextureCube::LoadFromSingleFile(const byte* pixels, uint mbits)
	{
		utils::LogUtil::WriteLine("ERROR", "[GLTextureCube] LoadFromSingleFile() - Load from single file is not implemented");

#ifdef GE_DEBUG
		__debugbreak();
#endif

		if (pixels != nullptr)
			gedel[] pixels;

		return 0;
	}

	uint GLTextureCube::LoadFromMultipleFiles(const byte** sides, uint mbits)
	{
		m_Parameters.format = TextureFormat::RGBA;

		uint width = m_Width[0];
		uint height = m_Height[0];
		uint bits = mbits;

		const byte* xp = sides[0];
		const byte* xn = sides[1];
		const byte* yp = sides[2];
		const byte* yn = sides[3];
		const byte* zp = sides[4];
		const byte* zn = sides[5];

		uint handle;

		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, handle));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		uint internalFormat = GLConvert::TextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, xp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, xn));

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, yp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, yn));

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, zp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, zn));

		GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		if (sides != nullptr)
		{
			for (uint i = 0; i < 6; i++)
				gedel[] sides[i];

			gedel[] sides;
		}

		return handle;
	}

	uint GLTextureCube::LoadFromVerticalCross(const byte** miptextures, uint* width, uint* height, uint mbits, uint mips)
	{
		uint srcWidth = width[0];
		uint srcHeight = height[0];
		uint bits = mbits;
		m_Mips = mips;

		byte*** cubeTextureData = genew byte**[mips];

		for (uint i = 0; i < mips; i++)
			cubeTextureData[i] = genew byte*[6];

		m_FaceWidths = genew uint[mips];
		m_FaceHeights = genew uint[mips];

		m_Parameters.format = TextureFormat::RGBA;

		for (uint i = 0; i < mips; i++)
		{
			const byte* data = miptextures[i];
			srcWidth = width[i];
			srcHeight = height[i];

			uint stride = bits / 8;

			uint face = 0;
			uint fWidth = srcWidth / 3;
			uint fHeight = srcHeight / 4;

			m_FaceWidths[i] = fWidth;
			m_FaceHeights[i] = fHeight;

			for (uint cy = 0; cy < 4; cy++)
			{
				for (uint cx = 0; cx < 3; cx++)
				{
					if (cy == 0 || cy == 2 || cy == 3)
						if (cx != 1)
							continue;

					cubeTextureData[i][face] = genew byte[fWidth * fHeight * stride];
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

							uint xp = cx * fWidth + offset;

							uint xyws = (x + y * fWidth) * stride;
							uint sxyws = (xp + yp * srcWidth) * stride;

											cubeTextureData[i][face][xyws + 0] = data[sxyws + 0];
											cubeTextureData[i][face][xyws + 1] = data[sxyws + 1];
											cubeTextureData[i][face][xyws + 2] = data[sxyws + 2];
							if(stride >= 4) cubeTextureData[i][face][xyws + 3] = data[sxyws + 3];
						}
					}

					face++;
				}
			}

			gedel[] data;
		}

		uint handle;

		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, handle));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		uint internalFormat = GLConvert::TextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;

		for (uint i = 0; i < mips; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][3]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][1]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][0]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][4]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][2]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][5]));
		}

		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		for (uint i = 0; i < mips; i++)
		{
			for (uint j = 0; j < 6; j++)
				gedel[] cubeTextureData[i][j];
			gedel[] cubeTextureData[i];
		}
		gedel[] cubeTextureData;

		return handle;
	}

	uint GLTextureCube::LoadFromVerticalCross(const byte*** cubeTextureData, uint* faceWidths, uint* faceHeights, uint bits, uint mips)
	{
		uint handle;

		m_FaceWidths = faceWidths;
		m_FaceHeights = faceHeights;
		m_BitsPerPixel = bits;
		m_Mips = mips;

		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, handle));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		uint internalFormat = GLConvert::TextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;

		for (uint i = 0; i < mips; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][3]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][1]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][0]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][4]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][2]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, internalFormat, m_FaceWidths[i], m_FaceHeights[i], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[i][5]));
		}

		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		for (uint i = 0; i < mips; i++)
		{
			for (uint j = 0; j < 6; j++)
				gedel[] cubeTextureData[i][j];
			gedel[] cubeTextureData[i];
		}
		gedel[] cubeTextureData;

		return handle;
	}


	void GLTextureCube::getPixelData(byte*** cubeTextureData)
	{
		Bind();

		uint internalFormat = GLConvert::TextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;

		for (uint i = 0; i < m_Mips; i++)
		{
			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][3]));
			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][1]));

			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][0]));
			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][4]));

			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][2]));
			GLCall(glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, format, GL_UNSIGNED_BYTE, cubeTextureData[i][5]));
		}

		Unbind();
	}

	uint GLTextureCube::getSize() const
	{
		return m_BitsPerPixel;
	}

} }