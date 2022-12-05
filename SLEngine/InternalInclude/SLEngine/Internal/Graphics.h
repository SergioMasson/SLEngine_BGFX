#pragma once

struct GLFWwindow;


namespace SLEngine 
{
    namespace Graphics
    {
        void Initialize(GLFWwindow *window);
        void Resize(GLFWwindow *window);
        void Render();
        void Shutdown();
    }
}