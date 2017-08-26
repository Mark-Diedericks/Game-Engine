#include "Pch.h"
#include "Application/Application.h"
#include "Application/Window.h"
#include "Input/InputManager.h"
#include "Events/WindowEvent.h"
#include "Graphics/Renderer/Renderer.h"

#define NOMINMAX
#undef NOGDI
#include <Windows.h>
#include <Windowsx.h>
#define NOGDI

#include <GL/glew.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace gebase {
	using namespace events;
	using namespace graphics;
	using namespace input;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	extern void MouseButtonCallback(InputManager* inputManager, int32 button, int32 x, int32 y);
	extern void KeyCallback(InputManager* inputManager, int32 flags, int32 keyCode, uint message);

	HINSTANCE hInstance;
	HDC hDc;
	HWND hWnd;

	static PIXELFORMATDESCRIPTOR getPixelFormat()
	{
		PIXELFORMATDESCRIPTOR res = {};
		res.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		res.nVersion = 1;
		res.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		res.iPixelType = PFD_TYPE_RGBA;
		res.cColorBits = 32;
		res.cDepthBits = 24;
		res.cStencilBits = 8;
		res.cAuxBuffers = 0;
		res.iLayerType = PFD_MAIN_PLANE;
		return res;
	}

	bool gebase::Window::PlatformInit()
	{
		hInstance = (HINSTANCE)&__ImageBase;

		WNDCLASS wndClass = {};
		wndClass.hInstance = hInstance;
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.lpfnWndProc = (WNDPROC)WndProc;
		wndClass.lpszClassName = "Game Engine Base Window - Win32";
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassA(&wndClass))
		{
			std::cout << "Could not register the Win32 Window Class." << std::endl;
			return false;
		}

		RECT size = { 0, 0, (LONG)m_Properties.width, (LONG)m_Properties.height };
		AdjustWindowRectEx(&size, WS_EX_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		hWnd = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, wndClass.lpszClassName, m_Title.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, (int)(GetSystemMetrics(SM_CXSCREEN) / 2.0f - m_Properties.width / 2.0f), (int)(GetSystemMetrics(SM_CYSCREEN) / 2.0f - m_Properties.height / 2.0f), (int)(size.right + (-size.left)), (int)(size.bottom + (-size.top)), NULL, NULL, hInstance, NULL);

		if (!hWnd)
		{
			std::cout << "Failed to create the window handle." << std::endl;
			return false;
		}

		RegisterWindowClass(hWnd, this);

		hDc = GetDC(hWnd);
		PIXELFORMATDESCRIPTOR pfd = getPixelFormat();
		int32 pf = ChoosePixelFormat(hDc, &pfd);

		if (pf)
		{
			if (!SetPixelFormat(hDc, pf, &pfd))
			{
				std::cout << "Failed to set the pixel format." << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "Failed to choose the pixel format." << std::endl;
			return false;
		}

		graphics::Context::Create(m_Properties, hWnd);

		ShowWindow(hWnd, SW_SHOW);
		SetFocus(hWnd);
		
		return true;
	}

	void gebase::Window::PlatformUpdate()
	{
		MSG message;

		while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0)
		{
			if (message.message == WM_QUIT)
			{
				m_Closed = true;
				return;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		m_InputManager->PlatformUpdate();
		Renderer::Present();
	}

	void gebase::Window::setTitle(const String& title)
	{
#if defined(GE_DEBUG)
		m_Title = title + " | " + Application::getApplication().getPlatform();
#else
		m_Title = title;
#endif

		SetWindowText(hWnd, m_Title.c_str());
	}

	void ResizeCallback(gebase::Window* window, int32 width, int32 height)
	{
		window->m_Properties.width = width;
		window->m_Properties.height = height;
		FontManager::SetScale(math::Vector2f((float)width / 32.0f, (float)height / 18.0f));

		if (window->m_EventCallback)
			window->m_EventCallback(events::WindowResizeEvent((uint)width, (uint)height));
	}

	void FocusCallback(gebase::Window* window, bool focused)
	{
		if (!focused)
		{
			window->m_InputManager->clearKeys();
			window->m_InputManager->clearMouseButtons();
		}
	}
}