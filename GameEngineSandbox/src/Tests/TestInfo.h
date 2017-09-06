#pragma once
#include <GameEngineBase.h>

class TestInfo : public gebase::graphics::Layer2D
{
private:
	gebase::graphics::Label** m_DebugInfo;
	gebase::graphics::Renderer2D* m_Renderer;
public:
	TestInfo();
	~TestInfo();

	void OnInit(gebase::graphics::Renderer2D& renderer, gebase::graphics::Material& material) override;

	void OnUpdate(float delta) override;
	void OnRender(gebase::graphics::Renderer2D& renderer) override;

	bool OnKeyPressedEvent(gebase::events::KeyPressedEvent& event);
	bool OnMousePressedEvent(gebase::events::MousePressedEvent& event);
	void OnEvent(gebase::events::Event& event) override;
};