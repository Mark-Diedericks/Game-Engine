#pragma once

#include <GameEngineBase.h>

class Test3D : public gebase::graphics::Layer3D
{
private:
	gebase::graphics::Camera* m_MayaCamera;
	gebase::graphics::Camera* m_FPSCamera;

	gebase::entity::Entity* m_Cube;
	std::vector<gebase::entity::Entity*> m_Spheres;
	gebase::entity::Entity* m_Plane;
	gebase::entity::Entity* m_Dagger;

	gebase::graphics::PBRMaterial* m_DaggerMaterial;
	gebase::graphics::MaterialInstance* m_SkyboxMaterial;
	gebase::graphics::Shader* m_PBRShader;
	gebase::graphics::TextureCube* m_EnvironmentMap;
	gebase::graphics::Light* m_Light;

	float m_Rotation;
	bool m_SetUniforms[2];
	std::vector<gebase::graphics::PBRMaterial*> m_Materials;
public:
	Test3D();
	~Test3D();

	void OnInit(gebase::graphics::Renderer3D& renderer, gebase::Scene& scene) override;

	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnEvent(gebase::events::Event& event) override;
};