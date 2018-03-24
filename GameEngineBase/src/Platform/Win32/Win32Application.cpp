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

		const float updateInterval = 1000.0f / 60.0f;
		const float tickInterval = 1000.0f / updateInterval;

		float now = 0.0;
		float previous = m_Timer->ElapsedMillis();

		float updatePrevious = m_Timer->ElapsedMillis();
		float updateDelta = 0.0f;

		unsigned int frameCount = 0;
		unsigned int updateCount = 0;

		while (m_Running)
		{
			now = m_Timer->ElapsedMillis();
			utils::LogUtil::WriteLine("PERFORMANCE", "-----------------------------------------------------------------------------------------------------------------------------------------");

			GE_PERF(window->Clear())

			updateDelta = now - updatePrevious;

			if (updateDelta >= updateInterval)
			{
				//Calculate current frame time
				m_FrT = now - previous;

				//Update 60 times a second
				GE_PERF(OnUpdate(updateDelta))
				updateCount++;
				updatePrevious += updateInterval;

				//Check 60 times a second
				if (window->Closed())
					m_Running = false;

				//Tick 1 time a second
				if (updateCount >= tickInterval) {
					m_FPS = frameCount;
					m_UPS = updateCount;

					frameCount = 0;
					updateCount = 0;

					GE_PERF(OnTick());

					utils::LogUtil::WriteLine("INFO", "Render Stats || FrameTime: " + StringFormat::Float((float)m_FrT) + "ms || UPS: " + std::to_string(m_UPS) + " || FPS: " + std::to_string(m_FPS));
				}
			}

			GE_PERF(OnRender())
			GE_PERF(window->Update())
			
			frameCount++;
			previous = now;
		}
	}

	String Application::getPlatform()
	{
		return "Windows";
	}
}