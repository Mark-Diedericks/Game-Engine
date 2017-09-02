#include "Test2D.h"

using namespace gebase;
using namespace graphics;
using namespace events;
using namespace entity;
using namespace component;
using namespace math;

Test2D::Test2D()
	: Layer2D(genew Scene2D(Matrix4f::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)))
{
	m_Renderer = nullptr;
}

Test2D::~Test2D()
{

}

void Test2D::OnInit(Renderer2D& renderer, Material& material)
{
	// m_Window->SetVsync(false);
	m_Renderer = &renderer;

	renderer.setRenderTarget(RenderTarget::SCREEN);
	//renderer.AddPostEffectsPass(new PostEffectsPass(Shader::CreateFromFile("Horizontal Blur", "shaders/postfx.shader")));
	//renderer.SetPostEffects(false);

	API::TextureParameters params;
	params.filter = API::TextureFilter::NEAREST;
	Add(genew Sprite(4.0f, 4.0f, 4, 4, Texture2D::CreateFromFile("Tex", "res/tb.png", params)));
	Add(genew Sprite(-5.0f, -5.0f, 3, 3, 0xffff00ff));

	FontManager::Add(genew Font("Consolas", "res/consola.ttf", 96));
	FontManager::Add(genew Font("Brush Script", "res/BrushScriptStd.otf", 96));

	m_DebugInfo = genew Label*[10];
	Add(m_DebugInfo[0] = genew Label("", -15.5f, 8.5f, 0xffffffff));
	Add(m_DebugInfo[1] = genew Label("", -15.5f, 7.5f, 0xffffffff));
	Add(m_DebugInfo[2] = genew Label("", -15.5f, 6.5f, 0xffffffff));
	Add(m_DebugInfo[3] = genew Label("", -15.5f, 5.5f, 0xffffffff));
	Add(m_DebugInfo[4] = genew Label("", -15.5f, 4.5f, 0xffffffff));

	Add(genew Label("Consolas", -15.5f, 0.0f, FontManager::Get("Consolas"), 0xffffffff));
	Add(genew Label("Brush Script", -15.5f, 2.0f, FontManager::Get("Brush Script"), 0xffffffff));

	Texture::setWrap(API::TextureWrap::CLAMP_TO_BORDER);
	Mask* mask = genew Mask(Texture2D::CreateFromFile("Mask", "res/mask.png"));
	mask->transform = Matrix4f::Translation(Vector3f(-16.0f, -9.0f, 0.0f)) * Matrix4f::Scale(32, 18, 1);
	setMask(mask);
}


void Test2D::OnUpdate(float delta)
{

	Application& app = Application::getApplication();
	//SP_INFO(app.GetUPS(), " ups, ", app.GetFPS(), " fps");
	//std::cout << "[Test2D] OnUpdate() - FPS: " << app.getFPS() << " UPS: " << app.getUPS() << std::endl;

	m_DebugInfo[2]->setText("Total Allocs: " + StringFormat::ToString(MemoryManager::Get()->GetMemoryInfo().tAllocations));
	m_DebugInfo[3]->setText("Total Allocated: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tAllocated));
	m_DebugInfo[4]->setText("Total Freed: " + MemoryManager::BytesToString(MemoryManager::Get()->GetMemoryInfo().tFreed));
}

bool Test2D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (!m_Renderer)
		return false;

	Renderer2D& renderer = *m_Renderer;

	if (event.getRepeat())
		return false;

	if (event.getKeyCode() == GE_KEY_T)
	{
		renderer.setRenderTarget(renderer.getRenderTarget() == RenderTarget::SCREEN ? RenderTarget::BUFFER : RenderTarget::SCREEN);
		return true;
	}

	if (event.getKeyCode() == GE_KEY_P)
	{
		renderer.setPostEffects(!renderer.getPostEffects());
		return true;
	}

	if (event.getKeyCode() == GE_KEY_0)
	{
		Application::getApplication().TrySetRenderAPI(graphics::Context::getRenderAPI() == graphics::RenderAPI::D3D11 ? graphics::RenderAPI::OPENGL : graphics::RenderAPI::D3D11);
		return true;
	}

	return false;
}

bool Test2D::OnMousePressedEvent(MousePressedEvent& event)
{
	return false;
}

void Test2D::OnEvent(gebase::events::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&Test2D::OnKeyPressedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&Test2D::OnMousePressedEvent));
}

void Test2D::OnRender(Renderer2D& renderer)
{
	m_DebugInfo[0]->setText(String("Target: ") + (renderer.getRenderTarget() == RenderTarget::SCREEN ? "Screen" : "Buffer"));
	m_DebugInfo[1]->setText(String("PostFX: ") + (renderer.getPostEffects() ? "On" : "Off"));
}
