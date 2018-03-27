#pragma once

#include "Common.h"
#include "Layer.h"
#include "Graphics/Scene.h"
#include "Graphics/Renderer/Renderer3D.h"
#include "Graphics/Renderer/ForwardRenderer.h"

namespace gebase { namespace graphics {

	class GE_API Layer3D : public Layer
	{
	protected:
		Scene* m_Scene;
		Renderer3D* m_Renderer;

		virtual bool OnResize(uint width, uint height) override;
	public:
		Layer3D(Scene* scene, Renderer3D* renderer = genew ForwardRenderer());
		~Layer3D();

		virtual void Init();
		virtual void OnInit(Renderer3D& renderer, Scene& scene);
		virtual void OnRender(Renderer3D& renderer);

		inline Scene* getScene() const { return m_Scene; }

		void OnUpdateInternal(const float delta) override;
		void OnRender() override;
	};

} }