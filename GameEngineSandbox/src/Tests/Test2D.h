#pragma once

#include <GameEngineBase.h>

class Test2D : public gebase::graphics::Layer2D
{
private:
	gebase::graphics::Renderer2D* m_Renderer;
public:
	Test2D();
	~Test2D();

	void OnInit(gebase::graphics::Renderer2D& renderer, gebase::graphics::Material& material) override;

	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender(gebase::graphics::Renderer2D& renderer) override;

	bool OnKeyPressedEvent(gebase::events::KeyPressedEvent& event);
	bool OnMousePressedEvent(gebase::events::MousePressedEvent& event);
	void OnEvent(gebase::events::Event& event) override;
};