#pragma once

#include <GameEngineBase.h>

class SponzaTest : public gebase::graphics::Layer3D
{
private:
	gebase::graphics::Camera* m_MayaCamera;
	gebase::graphics::Camera* m_FPSCamera;
	gebase::graphics::MaterialInstance* m_SkyboxMaterial;

public:
	SponzaTest();
	~SponzaTest();

	void OnInit(gebase::graphics::Renderer3D& renderer, gebase::Scene& scene) override;

	void OnUpdate(float delta) override;
	void OnRender(gebase::graphics::Renderer3D& renderer);

	void OnEvent(gebase::events::Event& event) override;
};