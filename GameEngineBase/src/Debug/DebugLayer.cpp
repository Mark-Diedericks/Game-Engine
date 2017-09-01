#include "ge.h"
#include "DebugLayer.h"

#include "Graphics/Shader/ShaderFactory.h"
#include "Graphics/Label.h"
#include "Graphics/Sprite.h"

#include "Math/Maths.h"
#include "System/MemoryManager.h"
#include "Embedded/Embedded.h"

namespace gebase { namespace debug {

	using namespace math;
	using namespace graphics;
	using namespace events;
	
	DebugLayer* DebugLayer::s_Instance = nullptr;

	DebugLayer::DebugLayer() : Layer2D(Matrix4f::initOrthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f)), m_Application(Application::getApplication())
	{
		s_Instance = this;
	}

	DebugLayer::~DebugLayer()
	{
	}

	void DebugLayer::OnInit(Renderer2D& renderer, Material& material)
	{
		renderer.setRenderTarget(RenderTarget::SCREEN);

		m_FPSLabel = genew Label("", 31.5f, 17.5f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);
		m_FrametimeLabel = genew Label("", 31.5f, 16.8f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);
		m_MemoryUsageLabel = genew Label("", 31.5f, 16.1f, FontManager::Get(24), 0xffffffff, Label::Alignment::RIGHT);

		Add(m_FPSLabel);
		Add(m_FrametimeLabel);
		Add(m_MemoryUsageLabel);
	}

	void DebugLayer::OnUpdate(float delta)
	{
		DebugMenu::Get()->OnUpdate(delta);

		m_FPSLabel->setText(StringFormat::ToString(m_Application.getFPS()) + " fps");
		m_FrametimeLabel->setText(StringFormat::ToString(m_Application.getFrT()) + " ms");
		m_MemoryUsageLabel->setText(MemoryManager::BytesToString(MemoryManager().Get()->GetMemoryInfo().cUsed));
	}

	void DebugLayer::OnRender(Renderer2D& renderer)
	{
		if (DebugMenu::isVisible())
			DebugMenu::Get()->OnRender(renderer);

		for (uint i = 0; i < m_TempSprites.size(); i++)
			gedel m_TempSprites[i];

		m_TempSprites.clear();
	}

	void DebugLayer::DrawSprite(Sprite* sprite)
	{
		s_Instance->Submit(sprite);
	}

	void DebugLayer::DrawTexture(Texture* texture, const Vector2f& pos, const Vector2f& size)
	{
		Sprite* sprite = genew Sprite(pos.x, pos.y, size.x, size.y, texture);
		s_Instance->m_TempSprites.push_back(sprite);
		s_Instance->Submit(sprite);
	}

	void DebugLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MousePressedEvent>(METHOD(&DebugLayer::OnMousePressedEvent));
		dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&DebugLayer::OnMouseReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(METHOD(&DebugLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(METHOD(&DebugLayer::OnKeyPressedEvent));

		Layer::OnEvent(e);
	}

	bool DebugLayer::OnMousePressedEvent(MousePressedEvent& e)
	{
		return DebugMenu::isVisible() ? DebugMenu::Get()->OnMousePressed(e) : false;
	}

	bool DebugLayer::OnMouseReleasedEvent(MouseReleasedEvent& e)
	{
		return DebugMenu::isVisible() ? DebugMenu::Get()->OnMouseReleased(e) : false;
	}

	bool DebugLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		return false;
	}

	bool DebugLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.getRepeat())
			return false;

		if (e.getModifiers() == GE_MODIFIER_LEFT_CONTROL && e.getKeyCode() == GE_KEY_TAB)
		{
			DebugMenu::setVisible(!DebugMenu::isVisible());
			return true;
		}

		return false;
	}

} }
