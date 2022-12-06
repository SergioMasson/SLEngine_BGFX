#pragma once

namespace SLEngine
{
	namespace Debug
	{
		void Print(const char* msg);
		void Printf(const char* format, ...);

		void PrintSubMessage(const char* format, ...);
		void PrintSubMessage(void);
	}
}

#ifdef SLENGINE_RELEASE

#define SLENGINE_ASSERT( isTrue, ... ) (void)(isTrue)
#define SLENGINE_WARN_ONCE_IF( isTrue, ... ) (void)(isTrue)
#define SLENGINE_WARN_ONCE_IF_NOT( isTrue, ... ) (void)(isTrue)
#define SLENGINE_ERROR( msg, ... )
#define SLENGINE_DEBUGPRINT( msg, ... ) do {} while(0)

#else	// !SLE_RELEASE

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)
#define SLENGINE_ASSERT(isFalse, ... ) \
        if (!(bool)(isFalse)) { \
            SLEngine::Debug::Print("\nAssertion failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
            SLEngine::Debug::PrintSubMessage("\'" #isFalse "\' is false"); \
            SLEngine::Debug::PrintSubMessage(__VA_ARGS__); \
            SLEngine::Debug::Print("\n"); \
            __debugbreak(); \
        }

#define SLENGINE_WARN_ONCE_IF( isTrue, ... ) \
    { \
        static bool s_TriggeredWarning = false; \
        if ((bool)(isTrue) && !s_TriggeredWarning) { \
            s_TriggeredWarning = true; \
            SLEngine::Debug::Print("\nWarning issued in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
            SLEngine::Debug::PrintSubMessage("\'" #isTrue "\' is true"); \
            SLEngine::Debug::PrintSubMessage(__VA_ARGS__); \
            SLEngine::Debug::Print("\n"); \
        } \
    }

#define SLENGINE_WARN_ONCE_IF_NOT( isTrue, ... ) SLE_WARN_ONCE_IF(!(isTrue), __VA_ARGS__)

#define SLENGINE_ERROR( ... ) \
        SLEngine::Debug::Print("\nError reported in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
        SLEngine::Debug::PrintSubMessage(__VA_ARGS__); \
        SLEngine::Debug::Print("\n");

#define SLENGINE_DEBUGPRINT(msg, ...) \
        SLEngine::Debug::Printf( msg "\n", ##__VA_ARGS__ );

#endif