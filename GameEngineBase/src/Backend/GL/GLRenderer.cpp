#include "ge.h"
#include "Common.h"
#include "GLConvert.h"
#include "GLRenderer.h"
#include "Utils\LogUtil.h"

#include <GL\glew.h>

namespace gebase { namespace graphics {

	GLRenderer::GLRenderer()
	{
		m_Context = GLContext::Get();
	}

	void GLRenderer::InitInternal()
	{
		setDepthTesting(true);
		setBlend(true);
		setBlendFunction(RendererBlendFunction::SOURCE_ALPHA, RendererBlendFunction::ONE_MINUS_SOURCE_ALPHA);

		utils::LogUtil::WriteLine("INFO", "-----------------------------------");
		utils::LogUtil::WriteLine("INFO", "               OPENGL              ");
		utils::LogUtil::WriteLine("INFO", "		" + (String)(char*)glGetString(GL_VERSION));
		utils::LogUtil::WriteLine("INFO", "		" + (String)(char*)glGetString(GL_VENDOR));
		utils::LogUtil::WriteLine("INFO", "		" + (String)(char*)glGetString(GL_RENDERER));
		utils::LogUtil::WriteLine("INFO", "-----------------------------------");

		//TODO - API INDEPENDENT
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glFrontFace(GL_CCW));
		GLCall(glCullFace(GL_BACK));

		m_RendererTitle = "OpenGL";
	}

	void GLRenderer::ClearInternal(uint buffer)
	{
		GLCall(glClear(GLConvert::RendererBufferToGL(buffer)));
	}

	void GLRenderer::ClearColorDepthInternal()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLRenderer::PresentInternal()
	{
		m_Context->Present();
	}

	void GLRenderer::setDepthTestingInternal(bool enabled)
	{
		if (enabled)
		{ 
			GLCall(glEnable(GL_DEPTH_TEST)); 
		}
		else
		{ 
			GLCall(glDisable(GL_DEPTH_TEST)); 
		}
	}

	void GLRenderer::setBlendInternal(bool enabled)
	{
		if (enabled)
		{ 
			GLCall(glEnable(GL_BLEND)); 
		}
		else
		{ 
			GLCall(glDisable(GL_BLEND)); 
		}
	}

	void GLRenderer::setBlendFunctionInternal(RendererBlendFunction source, RendererBlendFunction destination)
	{
		GLCall(glBlendFunc(GLConvert::RendererBlendFunctionToGL(source), GLConvert::RendererBlendFunctionToGL(destination)));
	}

	void GLRenderer::setBlendEquationInternal(RendererBlendEquation blendEquation)
	{
		utils::LogUtil::WriteLine("ERROR", "[GLRenderer] setBlendEquationInternal() - Blend equation setting not implemented.");
#ifdef GE_DEBUG
		__debugbreak();
#endif
	}

	void GLRenderer::setViewportInternal(uint x, uint y, uint width, uint height)
	{
		GLCall(glViewport(x, y, width, height));
	}
} }