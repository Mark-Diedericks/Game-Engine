#pragma once

#include "Common.h"
#include "Graphics/Renderable2D.h"

namespace gebase { namespace graphics {

	class GE_API Group : public Renderable2D
	{
	private:
		std::vector<Renderable2D*> m_Renderables;
		math::Matrix4f m_TransformationMatrix;
	public:
		Group(const math::Matrix4f& transform);
		~Group();

		void Add(Renderable2D* renderable);
		void Submit(Renderer2D* renderer) const override;

		bool PreEmployRenderAPI() override;
		bool EmployRenderAPI(RenderAPI api) override;

		math::Matrix4f& getTransformationMatrix() { return m_TransformationMatrix; }
	};

} }