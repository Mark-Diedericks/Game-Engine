#include "ge.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLTexture2D.h"

#include <GL\glew.h>
#include <FreeImage.h>

#include "Utils/ImageUtil.h"
#include "Utils\LogUtil.h"

#include "GLShader.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	GLTexture2D::GLTexture2D(uint width, uint height, TextureParameters parameters) : Texture2D(0), m_Filepath("NULL"), m_Width(width), m_Height(height), m_Parameters(parameters)
	{
		m_Handle = Load(NULL, NULL);
	}

	GLTexture2D::GLTexture2D(const String& name, const byte* pixels, uint width, uint height, uint bits, TextureParameters parameters) : Texture2D(1), m_Filepath(name), m_Width(width), m_Height(height), m_Parameters(parameters)
	{
		m_Handle = Load(pixels, bits);
	}

	GLTexture2D::~GLTexture2D()
	{
		GLCall(glDeleteTextures(1, &m_Handle));
	}

	uint GLTexture2D::Load(const byte* pixels, uint bits)
	{
		uint handle;

		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_2D, handle));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLConvert::TextureWrapToGL(m_Parameters.wrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLConvert::TextureWrapToGL(m_Parameters.wrap)));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvert::TextureFormatToGL(m_Parameters.format), m_Width, m_Height, 0, GLConvert::TextureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels ? pixels : NULL));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (bits != NULL)
			m_BitsPerPixel = bits;
		else
			m_BitsPerPixel = m_Parameters.format == TextureFormat::RGB ? 24 : 32;

		if (pixels != nullptr)
			gedel[] pixels;

		return handle;
	}

	void GLTexture2D::setData(const byte* pixels)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GLConvert::TextureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels));
	}

	void GLTexture2D::setData(uint color)
	{
#if 0
		uint stride = m_Parameters.format == TextureFormat::RGB ? 3 : 4;
		uint size = m_Width * m_Height * stride;
		byte* data = genew byte[size];

		for (uint i = 0; i < size; i += stride)
		{
							data[i + 0] = (color & 0xff);
							data[i + 1] = (color & 0xff00) >> 8;
							data[i + 2] = (color & 0xff0000) >> 16;
			if(stride == 4) data[i + 3] = (color & 0xff000000) >> 24;
		}

		setData(data);
		gedel[] data;
#else
		utils::LogUtil::WriteLine("ERROR", "[GLTexture2D] setData() - setData to color is broken!");
#ifdef GE_DEBUG
		__debugbreak();
#endif
#endif // 0

	}

	void GLTexture2D::Bind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
	}

	void GLTexture2D::Unbind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void GLTexture2D::getPixelData(byte* pixels)
	{
		Bind();
		GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GLConvert::TextureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels));
		Unbind();
	}

	uint GLTexture2D::getSize() const
	{
		return m_Width * m_Height * (m_Parameters.format == TextureFormat::RGB ? 3 : 4);
	}

} }