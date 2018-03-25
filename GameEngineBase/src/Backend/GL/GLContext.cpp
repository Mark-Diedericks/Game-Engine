#include "ge.h"
#include "Common.h"
#include "Utils/LogUtil.h"
#include "GLContext.h"

#undef NOGDI
#include <Windows.h>
#define NOGDI

#include <GL\glew.h>

namespace gebase { namespace graphics {

	static HDC hDc;
	static HGLRC hrc;
	static HWND hWnd;

	GLContext::GLContext(WindowProperties properties, void* deviceContext)
	{
		hWnd = (HWND)deviceContext;
		hDc = GetDC(hWnd);
		HGLRC hrc = wglCreateContext(hDc);

		if (hrc)
		{
			if (!wglMakeCurrent(hDc, hrc))
			{
				utils::LogUtil::WriteLine("ERROR", "Failed setting OpenGL context.");
#ifdef GE_DEBUG
				__debugbreak();
#endif
			}
		}
		else
		{
			utils::LogUtil::WriteLine("ERROR", "Failed creating OpenGL context.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		if (glewInit() != GLEW_OK)
		{
			utils::LogUtil::WriteLine("ERROR", "Could not initialize GLEW.");
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		setVSync(properties.vsync);
	}

	GLContext::~GLContext()
	{
		//TODO end glew

		HGLRC hgrlc;
		HDC hdc;

		if (hgrlc = wglGetCurrentContext())
		{
			hdc = wglGetCurrentDC();
			wglMakeCurrent(NULL, NULL);
			ReleaseDC(hWnd, hdc);
			wglDeleteContext(hgrlc);
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
		GE_PERF(SwapBuffers(hDc))
	}

} }