#include "ge.h"
#include "Layer2D.h"

#include "Graphics/Renderer/Renderer2D.h"
#include "Application/Window.h"
#include "Application/Application.h"

namespace gebase { namespace graphics {

	Layer2D::Layer2D(const math::Matrix4f& projectionMatrix)
	{
		uint width = Application::getApplication().getWindowWidth();
		uint height = Application::getApplication().getWindowHeight();

		m_Renderer = genew Renderer2D(width, height);
		m_Scene = genew Scene2D(projectionMatrix);
		m_Renderer->setCamera(m_Scene->getCamera());
	}

	Layer2D::Layer2D(Scene2D* scene) : m_Scene(scene)
	{
		uint width = Application::getApplication().getWindowWidth();
		uint height = Application::getApplication().getWindowHeight();

		m_Renderer = genew Renderer2D(width, height);
		m_Renderer->setCamera(m_Scene->getCamera());
	}

	Layer2D::~Layer2D()
	{
		gedel m_Material;
		gedel m_Renderer;
	}

	void Layer2D::Init()
	{
		OnInit(*m_Renderer, *m_Material);
	}

	void Layer2D::OnInit(Renderer2D& renderer, Material& material)
	{

	}

	Sprite* Layer2D::Add(Sprite* sprite)
	{
		m_Scene->Add(genew entity::Entity(sprite, math::Matrix4f::initTranslation(sprite->getPosition())));
		return sprite;
	}

	Renderable2D* Layer2D::Submit(Renderable2D* renderable)
	{
		m_SubmittedRenderables.push_back(renderable);
		return renderable;
	}

	bool Layer2D::OnResize(uint width, uint height)
	{
		m_Renderer->setScreenSize(math::Vector2f((float)width, (float)height));
		m_Scene->getRenderer()->setScreenSize(math::Vector2f((float)width, (float)height));
		return false;
	}

	void Layer2D::OnUpdateInternal(const float delta)
	{
		OnUpdate(delta);
	}

	void Layer2D::OnRender()
	{
		m_Scene->OnRender();
		m_Renderer->Begin();

		for (const Renderable2D* renderable : m_SubmittedRenderables)
			renderable->Submit(m_Renderer);

		m_Renderer->End();
		m_Renderer->Present();

		OnRender(*m_Renderer);
		m_SubmittedRenderables.clear();
	}

	void Layer2D::OnRender(Renderer2D& renderer)
	{

	}

} }