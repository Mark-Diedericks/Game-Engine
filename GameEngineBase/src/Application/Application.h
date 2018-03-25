#pragma once

#include "Common.h"
#include "CustomString.h"
#include "Window.h"
#include "Math/Maths.h"
#include "Graphics/Context.h"
#include "Graphics/Layer/Layer.h"
#include "Utils/Timer.h"

namespace gebase {
	namespace debug {
		class DebugLayer;
	}

	namespace graphics {
		class IRenderDependant;
	}

	class GE_API Application
	{
	private:
		static Application* s_Instance;
	public:
		Window* window;
		debug::DebugLayer* m_DebugLayer;

	private:
		Timer* m_Timer;
		bool m_Running;
		bool m_Suspended;
		uint m_UPS;
		uint m_FPS;
		float m_FrT;

		String m_LogDirectory;

		String m_Name;
		WindowProperties m_Properties;

		std::vector<graphics::Layer*> m_LayerStack;
		std::vector<graphics::Layer*> m_OverlayStack;
	public:
		Application(const String& name, const WindowProperties& properties, graphics::RenderAPI api = graphics::RenderAPI::OPENGL, const String& logDirectory = "./logs/");
		virtual ~Application();
		virtual void Create();

		void PushLayer(graphics::Layer* layer);
		graphics::Layer* PopLayer();
		graphics::Layer* PopLayer(graphics::Layer* layer);

		void PushOverlay(graphics::Layer* layer);
		graphics::Layer* PopOverlay();
		graphics::Layer* PopOverlay(graphics::Layer* layer);

		bool TrySetRenderAPI(graphics::RenderAPI api);

		void Start();
		void Stop();
		void Resume();
		void Suspend();

		inline uint getFPS() const { return m_FPS; }
		inline uint getUPS() const { return m_UPS; }
		inline float getFrT() const { return m_FrT; }

		inline uint getWindowWidth() const { return window->getWidth(); }
		inline uint getWindowHeight() const { return window->getHeight(); }
		inline uint getWindowCenterX() const { return window->getCenterX(); }
		inline uint getWindowCenterY() const { return window->getCenterY(); }

		inline static Application& getApplication() { return *s_Instance; }

		String getBuildConfig();
		String getPlatform();
	private:
		void PlatformInit();
		void Run();
		void OnTick();
		void OnUpdate(const float& delta);
		void OnRender();
		void OnEvent(events::Event& event);
	};
}