#pragma once

#include "ge.h"
#include "Common.h"
#include "Events/Event.h"
#include "Input/InputManager.h"

namespace gebase {

	typedef std::function<void(events::Event& event)> WindowEventCallback;

	struct GE_API WindowProperties
	{
		uint width;
		uint height;
		bool fullscreen;
		bool vsync;
	};

	class GE_API Window {
	private:
		friend void ResizeCallback(Window* window, int32 width, int32 height);
		friend void FocusCallback(Window* window, bool focused);

		static std::map<void*, Window*> s_Handles;
		String m_Title;
		WindowProperties m_Properties;
		bool m_Closed;
		void* m_Handle;

		bool m_VSync;
		WindowEventCallback m_EventCallback;
		input::InputManager* m_InputManager;
	private:
		bool Create();
		bool PlatformInit();
		void PlatformUpdate();
	public:
		Window(const String& name, const WindowProperties& properties);
		~Window();

		void Clear() const;
		void Update();
		bool Closed() const;

		void setTitle(const String& title);

		inline uint getWidth() { return m_Properties.width; }
		inline uint getHeight() { return m_Properties.height; }


		void setVSync(bool vsync);
		inline bool isVSync() { return m_VSync; }

		inline input::InputManager* getInputManager() const { return m_InputManager; }
		void setEventCallback(const WindowEventCallback& eventCallback);

		static void RegisterWindowClass(void* handle, Window* window);
		static Window* getWindowClass(void* handle);
	};
}