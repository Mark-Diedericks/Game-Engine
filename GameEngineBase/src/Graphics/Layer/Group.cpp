#include "ge.h"
#include "Group.h"
#include "Graphics/Renderer/Renderer2D.h"

namespace gebase { namespace graphics {

	Group::Group(const math::Matrix4f& transform) : Renderable2D(), m_TransformationMatrix(transform)
	{

	}

	Group::~Group()
	{
		for (uint i = 0; i < m_Renderables.size(); i++)
			gedel m_Renderables[i];
	}

	void Group::Add(Renderable2D* renderable)
	{
		m_Renderables.push_back(renderable);
	}

	void Group::Submit(Renderer2D* renderer) const
	{
		renderer->Push(m_TransformationMatrix);

		for (const Renderable2D* renderable : m_Renderables)
			renderable->Submit(renderer);

		renderer->Pop();
	}

	bool Group::PreEmployRenderAPI()
	{
		return true;
	}

	bool Group::EmployRenderAPI(RenderAPI api)
	{
		return true;
	}

} }