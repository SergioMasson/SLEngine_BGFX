#pragma once

namespace SLEngine
{
	class IGameApp
	{
	public:
		virtual ~IGameApp() {};
		// This function can be used to initialize application state and will run after essential
		// hardware resources are allocated.  Some state that does not depend on these resources
		// should still be initialized in the constructor such as pointers and flags.
		virtual void Startup(void) = 0;
		virtual void Cleanup(void) = 0;

		// Decide if you want the app to exit.  By default, app continues until the 'ESC' key is pressed.
		virtual bool IsDone(void) = 0;

		// The update method will be invoked once per frame.  Both state updating and scene
		// rendering should be handled by this method.
		virtual void Update(float deltaT) = 0;
	};

	void RunApplication(IGameApp& app, const char* className);
}

#if !TARGET_PLATFORM_WINDOWS_UWP
#define MAIN_FUNCTION()  int main(/*int argc, wchar_t** argv*/)
#else
#define MAIN_FUNCTION()  [Platform::MTAThread] int main(Platform::Array<Platform::String^>^)
#endif

#define CREATE_APPLICATION( app_class ) \
    MAIN_FUNCTION() \
    { \
        SLEngine::IGameApp* app = new app_class(); \
        SLEngine::RunApplication(*app, #app_class ); \
        delete app; \
        return 0; \
    }