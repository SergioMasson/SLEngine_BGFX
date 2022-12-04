#include "RobotsGame.h"
#include "SLEngine/Input.h"

#include <iostream>

void RobotsGame::Startup() 
{
}

bool RobotsGame::IsDone()
{
    return false;
}

void RobotsGame::Update(float deltaT)
{
    if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_0))
    {
        std::cout << "0 pressed" << std::endl;
    }
    else if (SLEngine::Input::IsPressed(SLEngine::Input::KeyCode::Key_1))
    {
        std::cout << "1 pressed" << std::endl;
    }
}

void RobotsGame::Cleanup()
{
}

