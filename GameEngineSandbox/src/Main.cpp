#include <GameEngineBase.h>

#include "Tests/TestInfo.h"
#include "Tests/Test2D.h"
#include "Tests/Test3D.h"
#include "Tests/SponzaTest.h"

using namespace gebase;
using namespace gebase::graphics;
using namespace gebase::math;

class Game : public Application
{
public:
	Game() : Application("Sandbox", { 1280, 720, 0, 0, false, false }, gebase::graphics::RenderAPI::OPENGL, "./logs/")
	{

	}

	Game::~Game()
	{

	}

	void Create() override
	{
		Application::Create();
		VirtualFileSystem::Get()->Mount("models", "res/models");
		VirtualFileSystem::Get()->Mount("pbr", "res/pbr");
		VirtualFileSystem::Get()->Mount("shaders", "shaders");

		//PushLayer(genew SponzaTest());
		PushLayer(genew Test3D());
		//PushLayer(genew Test2D());

		//PushOverlay(genew TestInfo());
	}

};

int main()
{
	Game game;
	game.Start();
	return 0;
}