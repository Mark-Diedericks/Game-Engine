#include "Pch.h"
#include "Common.h"
#include "Application.h"

#include "Debug/DebugLayer.h"
#include "Debug/DebugRenderer.h"

#include "System/System.h"
#include "System/Memory.h"

namespace gebase {
	using namespace graphics;
	using namespace debug;

	Application* Application::s_Instance = nullptr;

	void Application::Create()
	{
		System::Create();
		PlatformInit();

		DebugMenu::Create();
		m_DebugLayer = genew DebugLayer();
		m_DebugLayer->Create();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.push_back(layer);
		layer->Create();
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
		layer->Create();
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

		for (uint i = 0; i < m_OverlayStack.size(); i++)
			m_OverlayStack[i]->OnTick();

		for (uint i = 0; i < m_LayerStack.size(); i++)
			m_LayerStack[i]->OnTick();
	}

	void Application::OnUpdate(const float& delta)
	{
		m_DebugLayer->OnUpdate(delta);

		for (uint i = 0; i < m_OverlayStack.size(); i++)
			m_OverlayStack[i]->OnUpdateInternal(delta);

		for (uint i = 0; i < m_LayerStack.size(); i++)
			m_LayerStack[i]->OnUpdateInternal(delta);
	}

	void Application::OnRender()
	{
		for (uint i = 0; i < m_OverlayStack.size(); i++) 
		{
			if (m_OverlayStack[i]->isVisible())
				m_OverlayStack[i]->OnRender();
		}

		for (uint i = 0; i < m_LayerStack.size(); i++)
		{
			if(m_LayerStack[i]->isVisible())
				m_LayerStack[i]->OnRender();
		}

		Layer2D* debugLayer = (Layer2D*)m_DebugLayer;
		
		if (debugLayer->isVisible())
			debugLayer->OnRender();
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