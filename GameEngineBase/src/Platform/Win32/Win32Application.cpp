#include "Pch.h"
#include "Application/Application.h"

namespace gebase {

	Application::Application(const String& name, const WindowProperties& properties, graphics::RenderAPI api) : m_Name(name), m_Properties(properties), m_FrT(0.0f)
	{
		s_Instance = this;
		graphics::Context::setRenderAPI(api);
	}

	Application::~Application()
	{
		delete window;
	}

	void Application::PlatformInit()
	{
		window = new Window(m_Name, m_Properties);
		window->setEventCallback(METHOD(&Application::OnEvent));
	}

	void Application::Start()
	{
		Create();
		m_Running = true;
		m_Suspended = false;
		Run();
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::Suspend()
	{
		m_Suspended = true;
	}

	void Application::Resume()
	{
		m_Suspended = false;
	}

	void Application::Run()
	{
		m_Timer = utils::TimeUtil();

		double delta = 0.0;
		float counter = 0.0;
		long long avgFpsCount = 0;
		long secondsCount = 0;
		unsigned int fpsCount = 0;
		unsigned int fps = 0;
		unsigned int avgFps = 0;
		unsigned int minFps = 0;
		unsigned int maxFps = 0;

		while (m_Running)
		{
			delta = m_Timer.get();
			m_Timer.set();

			fpsCount++;
			counter += (float)delta;

			if (counter >= 1000.0f) {
				fps = (unsigned int)((double)fpsCount / (double)counter * 1000.0);
				avgFpsCount += fps;
				secondsCount++;

				m_FPS = fps;
				m_UPS = fps;
				m_FrT = ((float)counter / (float)fpsCount);

				avgFps = (unsigned int)((double)avgFpsCount / (double)secondsCount);

				if ((fps < minFps) || (minFps == 0))
					minFps = fps;

				if ((fps > maxFps) || (maxFps == 0))
					maxFps = fps;

				fpsCount = 0;
				counter = 0.0f;

				//std::cout << "Render Time: " << delta << "ms - FPS: " << fps << " MIN: " << minFps << " MAX: " << maxFps << " AVG: " << avgFps << std::endl;
			}

			window->Clear();
			OnUpdate((float)delta);
			OnRender();
			window->Update();
			OnTick();

			if (window->Closed())
				m_Running = false;
		}
	}

	String Application::getPlatform()
	{
		return "Windows";
	}
}