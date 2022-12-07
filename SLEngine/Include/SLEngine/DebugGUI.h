#pragma once

#include <string>

struct GLFWwindow;

namespace SLEngine
{
    struct DebugGUIFloat
    {
        float value;
    };

    namespace DebugGUI
    {
        DebugGUIFloat* CreateGUIFloat(float value, std::string text);
        void DeleteGUIFloat(DebugGUIFloat* debugFloat);
    }
}