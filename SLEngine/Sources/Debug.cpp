#include "SLEngine/Debug.h"
#include "bx/debug.h"

#include "iostream"

namespace SLEngine
{
	namespace Debug
	{
		void Print(const char* msg)
		{
			bx::debugOutput(msg);
		}

		void Printf(const char* format, ...)
		{
			char buffer[256];
			va_list ap;
			va_start(ap, format);
			vsprintf_s(buffer, 256, format, ap);
			va_end(ap);
			Print(buffer);
		}

		void PrintSubMessage(const char* format, ...)
		{
			Print("--> ");
			char buffer[256];
			va_list ap;
			va_start(ap, format);
			vsprintf_s(buffer, 256, format, ap);
			va_end(ap);
			Print(buffer);
			Print("\n");
		}

		void PrintSubMessage(void)
		{
		}
	}
}