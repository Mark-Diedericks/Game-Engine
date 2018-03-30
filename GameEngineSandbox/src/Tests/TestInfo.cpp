#include "TestInfo.h"

using namespace gebase;
using namespace graphics;
using namespace events;
using namespace entity;
using namespace component;
using namespace math;

TestInfo::TestInfo()
	: Layer2D(genew Scene2D(Matrix4f::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)))
{
	m_Renderer = nullptr;
}

TestInfo::~TestInfo()
{

}

void TestInfo::OnInit(Renderer2D& renderer, Material& material)
{
	m_Renderer = &renderer;

	renderer.setRenderTarget(RenderTarget::SCREEN);

	m_DebugInfo = genew Label*[10];
	Add(m_DebugInfo[0] = genew Label("", -15.5f, 8.5f, 0xffffffff));
	Add(m_DebugInfo[1] = genew Label("", -15.5f, 7.5f, 0xffffffff));
	Add(m_DebugInfo[2] = genew Label("", -15.5f, 6.5f, 0xffffffff));
	Add(m_DebugInfo[3] = genew Label("", -15.5f, 5.5f, 0xffffffff));
	Add(m_DebugInfo[4] = genew Label("", -15.5f, 4.5f, 0xffffffff));

	Texture::setWrap(TextureWrap::CLAMP_TO_BORDER); 

	m_DebugInfo[0]->setText("Total Alloctions: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryInfo().tAllocations));
	m_DebugInfo[1]->setText("Total Dealloctions: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryInfo().tDeallocations));
	m_DebugInfo[2]->setText("Total Allocated: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tAllocated));
	m_DebugInfo[3]->setText("Total Freed: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tFreed));

	String api = graphics::Context::getRenderAPI() == graphics::RenderAPI::D3D11 ? "Direct3D 11" : "OpenGL";
	m_DebugInfo[4]->setText("Render API: " + api);

	m_Visible = true;
}

void TestInfo::OnTick()
{
	m_DebugInfo[0]->setText("Total Alloctions: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryInfo().tAllocations));
	m_DebugInfo[1]->setText("Total Dealloctions: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryInfo().tDeallocations));
	m_DebugInfo[2]->setText("Total Allocated: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tAllocated));
	m_DebugInfo[3]->setText("Total Freed: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tFreed));

	String api = graphics::Context::getRenderAPI() == graphics::RenderAPI::D3D11 ? "Direct3D 11" : "OpenGL";
	m_DebugInfo[4]->setText("Render API: " + api);
}

void TestInfo::OnUpdate(float delta)
{
	
}

bool TestInfo::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (!m_Renderer)
		return false;

	if (event.getKeyCode() == GE_KEY_0)
	{
		Application::getApplication().TrySetRenderAPI(graphics::Context::getRenderAPI() == graphics::RenderAPI::D3D11 ? graphics::RenderAPI::OPENGL : graphics::RenderAPI::D3D11);
		return true;
	}

	return false;
}

bool TestInfo::OnMousePressedEvent(MousePressedEvent& event)
{
	return false;
}

void TestInfo::OnEvent(gebase::events::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestInfo::OnKeyPressedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&TestInfo::OnMousePressedEvent));
}

void TestInfo::OnRender(Renderer2D& renderer)
{

}
