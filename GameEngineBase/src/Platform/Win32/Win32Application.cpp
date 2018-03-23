#include "ge.h"
#include "Application/Application.h"
#include "Graphics/Context.h"
#include "Utils\LogUtil.h"

namespace gebase {

	Application::Application(const String& name, const WindowProperties& properties, graphics::RenderAPI api, const String& logDirectory) : m_Name(name), m_Properties(properties), m_FrT(0.0f), m_LogDirectory(logDirectory)
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
		//Render: Every frame
		//Update: Update Tick (60 times per second)
		//Tick: Once per second

		m_Timer = new Timer();

		const float updateTick = 1000.0f / 60.0f;
		const float statisticsInterval = 1000.0f;

		float now = 0.0;

		float fpsPrevious = m_Timer->ElapsedMillis();
		float upsPrevious = m_Timer->ElapsedMillis();
		float previous = m_Timer->ElapsedMillis();

		long long avgFpsCount = 0;
		long secondsCount = 0;
		unsigned int fpsCount = 0;
		unsigned int fps = 0;
		unsigned int avgFps = 0;
		unsigned int minFps = 0;
		unsigned int maxFps = 0;

		unsigned int upsCount = 0;
		unsigned int ups = 0;

		while (m_Running)
		{
			now = m_Timer->ElapsedMillis();

			fpsCount++;

			m_FrT = (float)now - previous;
			previous = now;

			window->Clear();

			if (now - upsPrevious >= updateTick)
			{
				OnUpdate(now - upsPrevious);
				upsCount++;
				upsPrevious += updateTick;
			}

			OnRender();
			window->Update();

			if (now - fpsPrevious >= statisticsInterval) {
				fps = (unsigned int)((double)fpsCount / ((double)(now - fpsPrevious) / 1000.0));
				ups = (unsigned int)((double)upsCount / ((double)(now - fpsPrevious) / 1000.0));

				avgFpsCount += fps;
				secondsCount++;

				m_FPS = fps;
				m_UPS = fps;

				avgFps = (unsigned int)((double)avgFpsCount / (double)secondsCount);

				if ((fps < minFps) || (minFps == 0))
					minFps = fps;

				if ((fps > maxFps) || (maxFps == 0))
					maxFps = fps;

				fpsCount = 0;
				upsCount = 0;

				fpsPrevious = now;

				OnTick();

				utils::LogUtil::WriteLine("INFO", "Render Stats || FrameTime: " + StringFormat::Float((float)m_FrT) + "ms || UPS: " + std::to_string(ups) + " || FPS: " + std::to_string(fps) + " MIN: " + std::to_string(minFps) + " MAX: " + std::to_string(maxFps) + " AVG: " + std::to_string(avgFps));
			}

			if (window->Closed())
				m_Running = false;
		}
	}

	String Application::getPlatform()
	{
		return "Windows";
	}
}