#include "ge.h"
#include "Common.h"

#include "GLFramebufferDepth.h"
#include "System/Memory.h"

#include <GL\glew.h>

namespace gebase { namespace graphics { namespace API {

	GLFramebufferDepth::GLFramebufferDepth(uint width, uint height) : m_Width(width), m_Height(height) { Init(); }

	GLFramebufferDepth::~GLFramebufferDepth()
	{
		GLCall(glDeleteFramebuffers(1, &m_FramebufferHandle));
	}

	void GLFramebufferDepth::Init()
	{
		GLCall(glGenFramebuffers(1, &m_FramebufferHandle));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));

		m_Texture = TextureDepth::Create(m_Width, m_Height);

		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ((API::GLTextureDepth*)m_Texture->getInstance())->getHandle(), 0));
		GLCall(glDrawBuffer(GL_NONE));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer (Depth) is incomplete" << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebufferDepth::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void GLFramebufferDepth::Unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebufferDepth::Clear()
	{
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	}

} } }