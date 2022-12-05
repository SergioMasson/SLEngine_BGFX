#include "SLEngine/GameCore.h"
#include "SLEngine/Internal/Input.h"
#include "SLEngine/Internal/Graphics.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define INITIAL_WIDTH 256
#define INITIAL_HEIGHT 256

GLFWwindow* g_windows = nullptr;

namespace SLEngine
{
    void InitializeApplication(IGameApp& game)
	{
		// GraphicsCore::Instance().Initialize();
		// SystemTime::Initialize();
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
		// float DeltaTime = GraphicsCore::Instance().GetFrameTime();
		Input::Update(0.016f);
		game.Update(0.016f);

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
			return;

	    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	    g_windows = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, className, NULL, NULL);

	    if (!g_windows)
	    {
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