#pragma once

struct GLFWwindow;

namespace SLEngine
{
    namespace DebugGUI
    {
        void Initialize(GLFWwindow* window);
        void Draw();
        void Shutdown();
    }
}