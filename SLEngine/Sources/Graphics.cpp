#include "SLEngine/Internal/Graphics.h"
#include "SLEngine/Debug.h"
#include "SLEngine/Internal/DebugGUI.h"

#include "bgfx/bgfx.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if SLENGINE_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#elif SLENGINE_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif SLENGINE_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif //
#include <GLFW/glfw3native.h>

#if SLENGINE_PLATFORM_OSX
#include "AppKit/AppKit.hpp"
#endif

static void* glfwNativeWindowHandle(GLFWwindow* _window)
{
#if SLENGINE_PLATFORM_LINUX
	return (void*)(uintptr_t)glfwGetX11Window(_window);
#elif SLENGINE_PLATFORM_OSX
	return ((NS::Window*)glfwGetCocoaWindow(_window));
#elif SLENGINE_PLATFORM_WINDOWS
	return glfwGetWin32Window(_window);
#endif // TARGET_PLATFORM_
}

static void* glfwNativeDisplayHandle()
{
#if SLENGINE_PLATFORM_LINUX
	return (void*)glfwGetX11Display();
#elif SLENGINE_PLATFORM_OSX
	return nullptr;
#elif SLENGINE_PLATFORM_WINDOWS
	return nullptr;
#endif // TARGET_PLATFORM_
}

static bgfx::RendererType::Enum getNativeArchType()
{
#if SLENGINE_PLATFORM_LINUX
	return bgfx::RendererType::Vulkan;
#elif SLENGINE_PLATFORM_OSX
	return bgfx::RendererType::Metal;
#elif SLENGINE_PLATFORM_WINDOWS
	return bgfx::RendererType::Direct3D11;
#endif // TARGET_PLATFORM_
}

uint32_t g_renderWidth = 0;
uint32_t g_renderHeight = 0;
uint32_t g_resetFlags = BGFX_RESET_VSYNC;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	g_renderWidth = static_cast<uint32_t>(width);
	g_renderHeight = static_cast<uint32_t>(height);

	bgfx::discard(BGFX_DISCARD_ALL);

	bgfx::reset(g_renderWidth, g_renderHeight, g_resetFlags);
	bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(g_renderWidth), static_cast<uint16_t>(g_renderHeight));
	bgfx::frame();
}

namespace SLEngine
{
	namespace Graphics
	{
		void Initialize(GLFWwindow* window)
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			g_renderWidth = static_cast<uint32_t>(width);
			g_renderHeight = static_cast<uint32_t>(height);

			bgfx::Init init;
			init.type = getNativeArchType();
			init.vendorId = BGFX_PCI_ID_NONE;
			init.platformData.nwh = glfwNativeWindowHandle(window);
			init.platformData.ndt = glfwNativeDisplayHandle();
			init.resolution.width = g_renderWidth;
			init.resolution.height = g_renderHeight;
			init.resolution.reset = g_resetFlags;

			auto succeed = bgfx::init(init);

			SLENGINE_ASSERT(succeed, "Fail to initialize bgfx");

			// Enable debug text.
			bgfx::setDebug(BGFX_DEBUG_TEXT);

			// Set view 0 clear state.
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

			glfwSetWindowSizeCallback(window, window_size_callback);
			DebugGUI::Initialize(window);
		}

		void Render()
		{
			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(g_renderWidth), uint16_t(g_renderHeight));

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			DebugGUI::Draw();

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();
		}

		void Shutdown()
		{
			// Shutdown bgfx.
			bgfx::shutdown();
			DebugGUI::Shutdown();
		}
	}
}