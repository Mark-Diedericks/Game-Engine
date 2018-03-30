#include "ge.h"
#include "Common.h"
#include "Application.h"

#include "Debug/DebugLayer.h"
#include "Debug/DebugRenderer.h"

#include "System/System.h"
#include "System/Memory.h"

#include "Utils/LogUtil.h"
#include "Graphics/BackendDependency/IRenderAPIDependant.h"
#include "Graphics/Framebuffer/FramebufferDepth.h"

namespace gebase {
	using namespace graphics;
	using namespace debug;

	Application* Application::s_Instance = nullptr;

	void Application::Create()
	{
		utils::LogUtil::Create(m_LogDirectory);
		System::Create();
		PlatformInit();

		DebugMenu::Init();
		m_DebugLayer = genew DebugLayer();
		m_DebugLayer->Init();
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->Init();
		m_LayerStack.push_back(layer);
	}

	Layer* Application::PopLayer()
	{
		Layer* layer = m_LayerStack.back();
		m_LayerStack.pop_back();
		return layer;
	}

	Layer* Application::PopLayer(Layer* layer)
	{
		for (uint i = 0; i < m_LayerStack.size(); i++)
		{
			if (m_LayerStack[i] == layer)
			{
				m_LayerStack.erase(m_LayerStack.begin() + i);
				break;
			}
		}
		return layer;
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_OverlayStack.push_back(layer);
		layer->Init();
	}

	Layer* Application::PopOverlay()
	{
		Layer* layer = m_OverlayStack.back();
		m_OverlayStack.pop_back();
		return layer;
	}

	Layer* Application::PopOverlay(Layer* layer)
	{
		for (uint i = 0; i < m_OverlayStack.size(); i++)
		{
			if (m_OverlayStack[i] == layer)
			{
				m_OverlayStack.erase(m_OverlayStack.begin() + i);
				break;
			}
		}

		return layer;
	}

	bool Application::TrySetRenderAPI(graphics::RenderAPI api)
	{
		m_PrevAPI = Context::getRenderAPI();

		if (!graphics::Context::EmployRenderAPI(api))
			return false;

		Context::FlushRenderAPIChange(m_PrevAPI);

		utils::LogUtil::WriteLine("INFO", "[Application] TrySetRenderAPI() - Render API Employment: Success!");

		return true;
	}

	void Application::OnEvent(events::Event& event)
	{
		m_DebugLayer->OnEvent(event);

		if (event.isHandled())
			return;

		for (int32 i = m_OverlayStack.size() - 1; i >= 0; i--)
		{
			m_OverlayStack[i]->OnEvent(event);
			
			if (event.isHandled())
				return;
		}

		for (int32 i = m_LayerStack.size() - 1; i >= 0; i--)
		{
			m_LayerStack[i]->OnEvent(event);

			if (event.isHandled())
				return;
		}
	}

	void Application::OnTick()
	{
		m_DebugLayer->OnTick();

		for (Layer* l : m_LayerStack)
			l->OnTick();

		for (Layer* o : m_OverlayStack)
			o->OnTick();
	}

	void Application::OnUpdate(const float& delta)
	{
		m_DebugLayer->OnUpdate(delta);

		for (Layer* l : m_LayerStack)
			l->OnUpdateInternal(delta);

		for (Layer* o : m_OverlayStack)
			o->OnUpdateInternal(delta);
	}

	void Application::OnRender()
	{
		GE_PERF({

			for (Layer* l : m_LayerStack)
			{
				if (l->isVisible())
					GE_PERF(l->OnRender())
			}

			for (Layer* o : m_OverlayStack)
			{
				if (o->isVisible())
					GE_PERF(o->OnRender())
			}
		})

		GE_PERF({
			if (m_DebugLayer->isVisible())
				((Layer2D*)m_DebugLayer)->OnRender();
		})
	}

	
	String Application::getBuildConfig()
	{
#if defined(GE_DEBUG)
		return "Debug Build Configuration";
#elif defined(GE_RELEASE)
		return "Release Build Configuration";
#else
		return "Invalid Build Configuration";
#endif
	}
}