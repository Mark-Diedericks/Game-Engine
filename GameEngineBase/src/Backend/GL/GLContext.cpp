#include "ge.h"
#include "Common.h"
#include "GLContext.h"

#undef NOGDI
#include <Windows.h>
#define NOGDI

#include <GL\glew.h>

namespace gebase { namespace graphics { namespace API {

	static HDC hDc;

	GLContext::GLContext(WindowProperties properties, void* deviceContext)
	{
		hDc = GetDC((HWND)deviceContext);
		HGLRC hrc = wglCreateContext(hDc);

		if (hrc)
		{
			if (!wglMakeCurrent(hDc, hrc))
			{
				std::cout << "Failed setting OpenGL context." << std::endl;
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}
		else
		{
			std::cout << "Failed creating OpenGL context." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
	}

	void GLContext::Present()
	{
		SwapBuffers(hDc);
	}

} } }