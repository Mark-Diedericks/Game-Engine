#include "ge.h"
#include "Common.h"
#include "GLVertexArray.h"

namespace gebase { namespace graphics { namespace API {

	GLVertexArray::GLVertexArray() 
	{

	}

	GLVertexArray::~GLVertexArray() 
	{
		
	}
	
	void GLVertexArray::Bind() const
	{
		m_Buffers.front()->Bind();
	}

	void GLVertexArray::Unbind() const
	{
		m_Buffers.front()->Unbind();
	}

	void GLVertexArray::Draw(uint count) const
	{
		std::cout << "glDrawElements... Begin... Count: " << count << std::endl;
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));
		std::cout << "glDrawElements... Ended... Count: " << count << std::endl;
	}

	void GLVertexArray::PushBuffer(gebase::graphics::VertexBuffer* buffer)
	{
		m_Buffers.push_back(buffer);
	}

} } }