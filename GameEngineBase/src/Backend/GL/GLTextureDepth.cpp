#include "ge.h"
#include "Common.h"

#include "GLTextureDepth.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	GLTextureDepth::GLTextureDepth(uint width, uint height) : m_Width(width), m_Height(height)
	{ 
		m_Handle = Init(); 
	}

	GLTextureDepth::~GLTextureDepth()
	{
		GLCall(glDeleteTextures(1, &m_Handle));
	}

	uint GLTextureDepth::Init()
	{
		uint handle;

		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_2D, handle));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		return handle;
	}

	void GLTextureDepth::Bind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
	}

	void GLTextureDepth::Unbind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void GLTextureDepth::setData(const uint16* pixels)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, pixels));
	}

	void GLTextureDepth::getPixelData(uint16* pixels)
	{
		Bind();
		GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, pixels));
		Unbind();
	}

	uint GLTextureDepth::getSize() const
	{
		return m_Width * m_Height * 1;
	}

} } }