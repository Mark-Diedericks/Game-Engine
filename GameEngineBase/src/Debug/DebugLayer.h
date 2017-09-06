#pragma once

#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/Label.h"
#include "Graphics/Sprite.h"

#include "Graphics/Layer/Layer2D.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Texture/Texture2D.h"

#include "Events/EventHeader.h"
#include "Application/Application.h"
#include "DebugMenu.h"

namespace gebase { namespace debug {

	class GE_API DebugLayer : public graphics::Layer2D
	{
	private:
		static DebugLayer* s_Instance;

		Application& m_Application;

		graphics::Label* m_FPSLabel;
		graphics::Label* m_MemoryUsageLabel;
		graphics::Label* m_FrametimeLabel;

		float m_TotalDelta;

		std::vector<graphics::Sprite*> m_TempSprites;
	public:
		DebugLayer();
		~DebugLayer();

		void OnInit(graphics::Renderer2D& renderer, graphics::Material& material) override;

		void OnUpdate(float delta) override;
		void OnRender(graphics::Renderer2D& renderer) override;

		void OnEvent(events::Event& e) override;
		bool OnMousePressedEvent(events::MousePressedEvent& e);
		bool OnMouseReleasedEvent(events::MouseReleasedEvent& e);
		bool OnMouseMovedEvent(events::MouseMovedEvent& e);
		bool OnKeyPressedEvent(events::KeyPressedEvent& e);

		inline static DebugLayer* Get() { return s_Instance; }

		static void DrawSprite(graphics::Sprite* sprite);
		static void DrawTexture(graphics::Texture* texture, const math::Vector2f& position = math::Vector2f(), const math::Vector2f& size = math::Vector2f());
	};

} }