#pragma once

#include "SLEngine/Input.h"

struct GLFWwindow;

namespace SLEngine
{
    namespace Input
    {
        // Initializes the input system for the target window.
        void Initialize(GLFWwindow *window);

        // Release all resources currently used by the input system.
        void Shutdown();

        // Update all reads for the input system.
        void Update(float frameDelta);

        // Cleans the reads for the current frame.
        void PostUpdate();

        void SetKey(int key, int scancode);
        void SetScrollValue(float value);
    }
}