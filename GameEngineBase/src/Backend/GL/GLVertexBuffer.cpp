#include "ge.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLVertexBuffer.h"
#include "Backend/API/APIConvert.h"

#include <GL\glew.h>

namespace gebase { namespace graphics {

	GLVertexBuffer::GLVertexBuffer(BufferUsage usage) : VertexBuffer(0), m_Usage(usage) 
	{
		GLCall(glGenBuffers(1, &m_Handle)) 
	}

	GLVertexBuffer::~GLVertexBuffer() 
	{ 
		GLCall(glDeleteBuffers(1, &m_Handle));
	}

	void GLVertexBuffer::Resize(uint size)
	{
		m_Size = size;

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GLConvert::BufferUsageToGL(m_Usage)));
	}

	void GLVertexBuffer::setLayout(const BufferLayout& bufferLayout)
	{
		m_Layout = bufferLayout;
		const std::vector<BufferElement>& layout = bufferLayout.getLayout();

		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count.gl_count, APIConvert::BufferElementTypeToGL(element.type), element.normalized, bufferLayout.getStride(), (const void*)element.offset));
		}
	}

	void GLVertexBuffer::setData(uint size, byte* data)
	{
		m_Size = size;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLConvert::BufferUsageToGL(m_Usage)));

		if (data != nullptr)
			gedel[] data;
	}

	void GLVertexBuffer::setData(uint size, Vertex* data)
	{
		m_Size = size;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLConvert::BufferUsageToGL(m_Usage)));
	}

	void GLVertexBuffer::setData(uint size, QuadVertex* data)
	{
		m_Size = size;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLConvert::BufferUsageToGL(m_Usage)));
	} 

	void GLVertexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		setLayout(m_Layout);
	}

	void GLVertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void GLVertexBuffer::ReleasePointer()
	{
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
	}

	void* GLVertexBuffer::getPointerInternal()
	{
		GLCall(void* result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		return result;
	}

	void GLVertexBuffer::getBufferData(void* data)
	{
		Bind();
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, data);
		Unbind();
	}

} }