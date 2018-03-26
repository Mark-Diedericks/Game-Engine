#include "ge.h"
#include "Common.h"

#include "GLIndexBuffer.h"
#include "System\Memory.h"

#include <GL\glew.h>

namespace gebase { namespace graphics {

	GLIndexBuffer::GLIndexBuffer(uint16* data, uint count) : IndexBuffer(0), m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_Handle));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint16), data, GL_STATIC_DRAW));

		if (data != nullptr)
			gedel[] data;
	}

	GLIndexBuffer::GLIndexBuffer(uint* data, uint count) : IndexBuffer(1), m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_Handle));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));

		if (data != nullptr)
			gedel[] data;
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_Handle));
	}

	void GLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
	}

	void GLIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void GLIndexBuffer::getIndexData32(uint* data)
	{
		Bind();
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Count * sizeof(uint), data);
		Unbind();
	}

	void GLIndexBuffer::getIndexData16(uint16* data)
	{
		Bind();
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Count * sizeof(uint16), data);
		Unbind();
	}

} }