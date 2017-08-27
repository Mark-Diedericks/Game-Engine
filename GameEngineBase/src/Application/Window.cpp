#include "ge.h"
#include "Window.h"
#include "Graphics/Context.h"
#include "Embedded/Embedded.h"
#include "Audio/SoundManager.h"

#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Texture/TextureManager.h"
#include "Graphics/Font/FontManager.h"

#include <FreeImage.h>

namespace gebase {
	using namespace graphics;
	using namespace audio;

	std::map<void*, Window*> Window::s_Handles;
	
	Window::Window(const String& title, const WindowProperties& properties) : m_Title(title), m_Properties(properties), m_Handle(nullptr), m_Closed(false), m_EventCallback(nullptr)
	{
		if (!Create())
		{
			std::cout << "[Window] Window() - Failed to initialize window base." << std::endl;
			return;
		}

		FontManager::setScale(math::Vector2f(m_Properties.width / 32.0f, m_Properties.height / 18.0f));
		FontManager::Add(new Font("SourceSansPro", embedded::DEFAULT_FONT, embedded::DEFAULT_FONT_SIZE, 32));

		FreeImage_Initialise();

		SoundManager::Create();
		m_InputManager = new InputManager();
	}

	Window::~Window()
	{
		FontManager::Clean();
		TextureManager::Clean();
		SoundManager::Destroy();
	}

	bool Window::Create()
	{
		if (!PlatformInit())
		{
			std::cout << "[Window] Create() - Failed the platform initialization." << std::endl;
			return false;
		}

		Renderer::Init();

		setTitle(m_Title);
		return true;
	}

	void Window::Clear() const
	{
		Renderer::Clear(API::RendererBufferType::RENDERER_BUFFER_COLOR | API::RendererBufferType::RENDERER_BUFFER_DEPTH);
	}

	void Window::Update()
	{
		PlatformUpdate();
		SoundManager::Update();
	}
	
	void Window::setEventCallback(const WindowEventCallback& eventCallback)
	{
		m_EventCallback = eventCallback;
		m_InputManager->setEventCallback(m_EventCallback);
	}

	void Window::RegisterWindowClass(void* handle, Window* window)
	{
		s_Handles[handle] = window;
	}

	Window* Window::getWindowClass(void* handle)
	{
		if (handle == nullptr)
			return s_Handles.begin()->second;

		return s_Handles[handle];
	}

	bool Window::Closed() const
	{
		return m_Closed;
	}

	void Window::setVSync(bool vsync)
	{
		m_VSync = vsync;
	}
}