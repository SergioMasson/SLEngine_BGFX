#include "SLEngine/GameCore.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if SLENGINE_PLATFORM_LINUX
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#elif SLENGINE_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#elif SLENGINE_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#endif //
#include <GLFW/glfw3native.h>

static void* glfwNativeWindowHandle(GLFWwindow* _window)
{
#if SLENGINE_PLATFORM_LINUX
	return (void*)(uintptr_t)glfwGetX11Window(_window);
#elif SLENGINE_PLATFORM_OSX
	return ((NSWindow*)glfwGetCocoaWindow(_window)).contentView;
#elif SLENGINE_PLATFORM_WINDOWS
	return glfwGetWin32Window(_window);
#endif // TARGET_PLATFORM_
}

#define INITIAL_WIDTH 256
#define INITIAL_HEIGHT 256

GLFWwindow* g_windows = nullptr;

namespace SLEngine
{
    void InitializeApplication(IGameApp& game)
	{
		// GraphicsCore::Instance().Initialize();
		// SystemTime::Initialize();
		// Input::Initialize();
		game.Startup();
	}

	void TerminateApplication(IGameApp& game)
	{
		game.Cleanup();
		//Input::Shutdown();
	}

	bool UpdateApplication(IGameApp& game)
	{
		// float DeltaTime = GraphicsCore::Instance().GetFrameTime();

		// Input::Update(DeltaTime);
		game.Update(0);

		// GraphicsCore::Instance().InitFrame();		
		//GraphicsCore::Instance().Present();

		return !game.IsDone();
	}

    void RunApplication(IGameApp& app, const char* className)
    {
        if (!glfwInit())
		exit(EXIT_FAILURE);

	    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	    g_windows = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, className, NULL, NULL);

	    if (!g_windows)
	    {
		    glfwTerminate();
		    exit(EXIT_FAILURE);
	    }

        InitializeApplication(app);

        while (!glfwWindowShouldClose(g_windows))
        {
            UpdateApplication(app);
            glfwPollEvents();
        }

        TerminateApplication(app);

        glfwDestroyWindow(g_windows);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}