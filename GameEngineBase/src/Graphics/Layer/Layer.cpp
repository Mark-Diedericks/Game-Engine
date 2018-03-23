#include "ge.h"
#include "Layer.h"

#include "Events/EventHeader.h"

namespace gebase { namespace graphics {

	Layer::Layer() : m_Window(Window::getWindowClass(nullptr)), m_Visible(true)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::Init()
	{

	}

	void Layer::OnTick()
	{

	}

	void Layer::OnEvent(events::Event& event)
	{
		events::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<events::WindowResizeEvent>([this](events::WindowResizeEvent& e) { return OnResize(e.getWidth(), e.getHeight()); });
	}

	bool Layer::OnResize(uint width, uint height)
	{
		return false;
	}

	void Layer::OnUpdate(const float delta)
	{

	}

	void Layer::OnUpdateInternal(const float delta)
	{

	}

	void Layer::OnRender()
	{

	}

} }