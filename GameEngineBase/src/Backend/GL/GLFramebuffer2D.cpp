#include "ge.h"
#include "Common.h"

#include "GLFramebuffer2D.h"
#include "System/Memory.h"

namespace gebase { namespace graphics { namespace API {

	using namespace API;

	GLFramebuffer2D::GLFramebuffer2D(uint width, uint height) : m_Width(width), m_Height(height) { Init(); }

	GLFramebuffer2D::~GLFramebuffer2D()
	{
		GLCall(glDeleteFramebuffers(1, &m_FramebufferHandle));
	}

	void GLFramebuffer2D::Init()
	{
		GLCall(glGenFramebuffers(1, &m_FramebufferHandle));
		GLCall(glGenFramebuffers(1, &m_DepthbufferHandle));

		TextureParameters params(TextureFormat::RGBA, TextureFilter::LINEAR, TextureWrap::CLAMP);
		m_Texture = Texture2D::Create(m_Width, m_Height, params);

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthbufferHandle));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_Width, m_Height));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((API::GLTexture2D*)m_Texture->getInstance())->getHandle(), 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthbufferHandle));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebuffer2D::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void GLFramebuffer2D::Unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebuffer2D::Clear() 
	{
		GLCall(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	byte* GLFramebuffer2D::getPixelData()
	{
		return m_Texture->getPixelData();
	}

	void GLFramebuffer2D::setData(const void* pixels)
	{
		m_Texture->setData(pixels);
	}

} } }