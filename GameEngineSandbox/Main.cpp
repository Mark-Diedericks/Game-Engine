#include <iostream>
#include <chrono>
#include "source/platform/win32/Window.h"
#include <GLFW\glfw3.h>
#include "src/math/Maths.h"
#include "source/graphics/opengl/Shader.h"
#include "source/graphics/opengl/GL_SimpleRenderer2D.h"
#include "source/graphics/Renderable2D.h"
#include "source/graphics/buffers/Buffer.h"
#include "source/graphics/buffers/IndexBuffer.h"
#include "source/graphics/buffers/VertexArray.h"
#include "src/Utils/TimeUtil.h"


int main() {
	using namespace platform;
	using namespace win32;
	using namespace math;
	using namespace graphics;
	using namespace opengl;
	using namespace input;
	using namespace utils;

	TimeUtil timer = TimeUtil();
	timer.set();

	double delta = 0.0;
	float counter = 0.0;
	long long avgFpsCount = 0;
	long secondsCount = 0;
	unsigned int fpsCount = 0;
	unsigned int fps = 0;
	unsigned int avgFps = 0;
	unsigned int minFps = 0;
	unsigned int maxFps = 0;

	Window window("3D Game Engine - OpenGL", 1280, 720);

	std::cout << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f* projection = Matrix4f::initOrthographic(0.0f, 16.0f, 9.0f, 0.0f, -1.0f, 10.0f);

	Shader shader("resources/shaders/basic.vs", "resources/shaders/basic.fs");
	shader.bind();
	shader.setUniformMat4("projection_matrix", *projection);

	GL_SimpleRenderer2D renderer(&shader);

	Transform transform(const_cast<Matrix4f*>(&Transform::IdentityMatrix));
	transform.setPosition(Vector3f(4, 3, 0));
	Renderable2D renderable(transform, Vector2f(8, 3), Vector4f(0.7f, 0.2f, 0.9f, 1.0f));

	while (!window.closed()) {

		delta = timer.get();
		timer.set();

		fpsCount++;
		counter += (float)delta;

		if (counter >= 1000.0f) {
			fps = (unsigned int)((double)fpsCount / (double)counter * 1000.0);
			avgFpsCount += fps;
			secondsCount++;

			avgFps = (unsigned int)((double)avgFpsCount / (double)secondsCount);

			if ((fps < minFps) || (minFps == 0))
				minFps = fps;

			if ((fps > maxFps) || (maxFps == 0))
				maxFps = fps;

			fpsCount = 0;
			counter = 0.0f;

			std::cout << "Render Time: " << delta << "ms - FPS: " << fps << " MIN: " << minFps << " MAX: " << maxFps << " AVG: " << avgFps << std::endl;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		shader.setUniformVec2("lightPos", Vector2f((float)Mouse::getX() * 16.0f / (float)window.getWidth(), 9.0f - ((float)Mouse::getY() * 9.0f / (float)window.getHeight())));
		renderer.submit(&renderable);
		renderer.flush();

		window.update();
	}

	window.~Window();
	std::cout << "Render Time: " << (int)delta << "ms - FPS: " << fps << " MIN: " << minFps << " MAX: " << maxFps << " AVG: " << avgFps << std::endl;
	system("PAUSE");
	return 0;
}