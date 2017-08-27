#include "ge.h"
#include "Layer3D.h"

//#include "Graphics/Renderer/DeferredRenderer.h"
#include "System/Memory.h"

namespace gebase { namespace graphics {

	Layer3D::Layer3D(Scene* scene, Renderer3D* renderer) : m_Scene(scene), m_Renderer(renderer)
	{

	}

	Layer3D::~Layer3D()
	{
		gedel m_Scene;
		gedel m_Renderer;
	}

	void Layer3D::Init()
	{
		OnInit(*m_Renderer, *m_Scene);
	}

	void Layer3D::OnInit(Renderer3D& renderer, Scene& scene)
	{

	}

	bool Layer3D::OnResize(uint width, uint height)
	{
		m_Renderer->setScreenBufferSize(width, height);
		return false;
	}

	void Layer3D::OnUpdateInternal(const float delta)
	{
		OnUpdate(delta);
	}

	void Layer3D::OnRender()
	{
		OnRender(*m_Renderer);
	}

	void Layer3D::OnRender(Renderer3D& renderer)
	{
		m_Scene->Render(renderer);
		renderer.Present();
	}

} }