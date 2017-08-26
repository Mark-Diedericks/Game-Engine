#include "Pch.h"
#include "Layer.h"

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

	void Layer::OnEvent(events::Event& event)
	{

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