#pragma once

#include "Layer.h"

#include "Common.h"
#include "Graphics/Scene2D.h"
#include "Events/Event.h"
#include "Graphics/Renderable2D.h"
#include "Graphics/Mask.h"

namespace gebase { namespace graphics {

	class GE_API Layer2D : public Layer
	{
	private:
		Renderer2D* m_Renderer;
	protected:
		Material* m_Material;
		Scene2D* m_Scene;
		std::vector<Renderable2D*> m_SubmittedRenderables;

		bool OnResize(uint width, uint height) override;
	public:
		Layer2D(const math::Matrix4f& projectionMatrix);
		Layer2D(Scene2D* scene);
		virtual ~Layer2D();

		virtual void Init();
		virtual void OnInit(Renderer2D& renderer, Material& material);

		virtual Sprite* Add(Sprite* sprite);
		virtual Renderable2D* Submit(Renderable2D* renderable);
		virtual void OnRender(Renderer2D& renderer);

		inline void setMask(const Mask* mask) const { m_Renderer->setMask(mask); }
		inline Scene2D* getScene() { return m_Scene; }

		void OnUpdateInternal(const float delta) override;
		void OnRender() override;
	};

} }