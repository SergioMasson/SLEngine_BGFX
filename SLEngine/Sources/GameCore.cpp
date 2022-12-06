#include "SLEngine/GameCore.h"
#include "SLEngine/Debug.h"
#include "SLEngine/Internal/Input.h"
#include "SLEngine/Internal/Graphics.h"
#include <chrono>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define INITIAL_WIDTH 256
#define INITIAL_HEIGHT 256

GLFWwindow* g_windows = nullptr;
std::chrono::high_resolution_clock::time_point g_frameTime;

namespace SLEngine
{
	void InitializeApplication(IGameApp& game)
	{
		// GraphicsCore::Instance().Initialize();
		Input::Initialize(g_windows);
		Graphics::Initialize(g_windows);
		game.Startup();
	}

	void TerminateApplication(IGameApp& game)
	{
		game.Cleanup();
		Input::Shutdown();
		Graphics::Shutdown();
	}

	bool UpdateApplication(IGameApp& game)
	{
		std::chrono::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - g_frameTime);
		double deltaT = duration.count();
		double fps = 1.0 / deltaT;
		Debug::Printf("FPS: %f \n", fps);
		g_frameTime = now;

		Input::Update(static_cast<float>(deltaT));
		game.Update(static_cast<float>(deltaT));

		glfwPollEvents();

		Input::PostUpdate();
		Graphics::Render();

		// GraphicsCore::Instance().InitFrame();
		//GraphicsCore::Instance().Present();

		return !game.IsDone() && !glfwWindowShouldClose(g_windows);
	}

	void RunApplication(IGameApp& app, const char* className)
	{
		if (!glfwInit())
		{
			SLENGINE_ASSERT(false, "glfwInit failed.");
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		g_windows = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, className, NULL, NULL);

		if (!g_windows)
		{
			SLENGINE_ASSERT(false, "glfwCreateWindow failed.");
			glfwTerminate();
			return;
		}

		InitializeApplication(app);

		while (UpdateApplication(app))
		{
		}

		TerminateApplication(app);

		glfwDestroyWindow(g_windows);
		glfwTerminate();

		return;
	}
}