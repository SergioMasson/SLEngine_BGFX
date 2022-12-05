#pragma once

#include "SLEngine/GameCore.h"

class RobotsGame : public SLEngine::IGameApp
{
public:
	~RobotsGame(){};
	// This function can be used to initialize application state and will run after essential
	// hardware resources are allocated.  Some state that does not depend on these resources
	// should still be initialized in the constructor such as pointers and flags.
	void Startup(void) override;
	void Cleanup(void) override;

	// Decide if you want the app to exit.  By default, app continues until the 'ESC' key is pressed.
	bool IsDone(void) override;

	// The update method will be invoked once per frame.  Both state updating and scene
	// rendering should be handled by this method.
	void Update(float deltaT) override;
};