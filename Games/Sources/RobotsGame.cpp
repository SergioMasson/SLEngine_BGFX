#include "RobotsGame.h"
#include "SLEngine/Input.h"
#include "SLEngine/DebugGUI.h"

#include <iostream>

SLEngine::DebugGUIFloat g_floatValue;

void RobotsGame::Startup() 
{
    g_floatValue = SLEngine::DebugGUI::CreateGUIFloat(1.0f, "DEBUG GUI TEST", SLEngine::FloatWidget::Drag);
}

bool RobotsGame::IsDone()
{
    return false;
}

void RobotsGame::Update(float deltaT)
{
    if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_p))
    {
        std::cout << "DEBUG GUI TEST: " << (float)g_floatValue << std::endl;
    }
    else if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_r))
    {
        g_floatValue.Set(0.0f);
    }
}

void RobotsGame::Cleanup()
{
}

