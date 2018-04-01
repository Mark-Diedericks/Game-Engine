#include "ge.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLVertexBuffer.h"
#include "Graphics/Context/ConversionUtil.h"

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

	void GLVertexBuffer::setLayout(const BufferLayout& bufferLayout, const Shader* shader)
	{
		m_Layout = bufferLayout;
		const std::vector<BufferElement>& layout = bufferLayout.getLayout();

		if (shader != nullptr)
			m_Shader = (GLShader*)shader;

		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count.gl_count, ConversionUtil::BufferElementTypeToGL(element.type), element.normalized, bufferLayout.getStride(), (const void*)element.offset));
		}
	}

	void GLVertexBuffer::setData(uint size, byte* data, bool del)
	{
		m_Size = size;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLConvert::BufferUsageToGL(m_Usage)));

		if (data != nullptr && del)
			gedel[] data;
	}

	void GLVertexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));

		const std::vector<BufferElement>& layout = m_Layout.getLayout();
		for (uint i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count.gl_count, ConversionUtil::BufferElementTypeToGL(element.type), element.normalized, m_Layout.getStride(), (const void*)element.offset));
		}
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
		//https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
		//https://www.gamedev.net/forums/topic/631265-slow-sprite-batch-rendering/
		//GLCall(void* result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		GLCall(void* result = glMapBufferRange(GL_ARRAY_BUFFER, 0, getSize(), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT));
		return result;
	}

	void GLVertexBuffer::getBufferData(byte* data)
	{
		Bind();
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, data);
		Unbind();
	}

} }