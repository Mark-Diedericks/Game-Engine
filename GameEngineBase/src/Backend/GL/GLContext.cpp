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
		setVSync(properties.vsync);

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

	void GLContext::setVSync(bool enabled)
	{
		typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

		const char* extensions = (char*)glGetString(GL_EXTENSIONS);

		if (!extensions)
			return;

		if (strstr(extensions, "WGL_EXT_swap_control") == 0)
		{
			return;
		}
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

			if (wglSwapIntervalEXT)
				wglSwapIntervalEXT(enabled);
		}
	}

	void GLContext::Present()
	{
		SwapBuffers(hDc);
	}

} } }