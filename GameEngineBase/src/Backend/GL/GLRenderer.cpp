#include "Pch.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLRenderer.h"

#include <GL\glew.h>

namespace gebase { namespace graphics { namespace API {

	GLRenderer::GLRenderer()
	{
		m_Context = API::GLContext::Get();
	}

	void GLRenderer::InitInternal()
	{
		setDepthTesting(true);
		setBlend(true);
		setBlendFunction(RendererBlendFunction::SOURCE_ALPHA, RendererBlendFunction::ONE_MINUS_SOURCE_ALPHA);

		std::cout << "-----------------------------------" << std::endl;
		std::cout << "               OPENGL              " << std::endl;
		std::cout << "		" << glGetString(GL_VERSION) << std::endl;
		std::cout << "		" << glGetString(GL_VENDOR) << std::endl;
		std::cout << "		" << glGetString(GL_RENDERER) << std::endl;
		std::cout << "-----------------------------------" << std::endl;

		//TODO - API INDEPENDENT
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glFrontFace(GL_CCW));
		GLCall(glCullFace(GL_BACK));

		m_RendererTitle = "OpenGL";
	}

	void GLRenderer::ClearInternal(uint buffer)
	{
		GLCall(glClear(API::GLConvert::RendererBufferToGL(buffer)));
	}

	void GLRenderer::PresentInternal()
	{
		m_Context->Present();
	}

	void GLRenderer::setDepthTestingInternal(bool enabled)
	{
		if (enabled)
		{ GLCall(glEnable(GL_DEPTH_TEST)); }
		else
		{ GLCall(glDisable(GL_DEPTH_TEST)); }
	}

	void GLRenderer::setBlendInternal(bool enabled)
	{
		if (enabled)
		{ GLCall(glEnable(GL_BLEND)); }
		else
		{ GLCall(glDisable(GL_BLEND)); }
	}

	void GLRenderer::setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination)
	{
		GLCall(glBlendFunc(API::GLConvert::RendererBlendFunctionToGL(source), API::GLConvert::RendererBlendFunctionToGL(destination)));
	}

	void GLRenderer::setBlendEquationInternal(RendererBlendEquation blendEquation)
	{
		std::cout << "[GLRenderer] BLEND EQUATION NOT IMPLEMENTED" << std::endl;
#ifdef GE_DEBUG
		__debugbreak();
#endif
	}

	void GLRenderer::setViewportInternal(uint x, uint y, uint width, uint height)
	{
		GLCall(glViewport(x, y, width, height));
	}
} } }