#include "RobotsGame.h"
#include "SLEngine/Input.h"
#include "SLEngine/DebugGUI.h"

#include <iostream>

SLEngine::DebugGUIFloat* g_floatValue;

void RobotsGame::Startup() 
{
    g_floatValue = SLEngine::DebugGUI::CreateGUIFloat(1.0f, "DEBUG GUI TEST");
}

bool RobotsGame::IsDone()
{
    return false;
}

void RobotsGame::Update(float deltaT)
{
    if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_0))
    {
        std::cout << "DEBUG GUI TEST: " << g_floatValue->value << std::endl;
    }
    else if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_1))
    {
        std::cout << "1 pressed" << std::endl;
    }
}

void RobotsGame::Cleanup()
{
}

