#include "pch.h"
#include "engine.h"
#include "clock.h"
#include "events.h"


int main()
{
	
	Engine.init();
	Clock.init();
	Events.init();

	while (Engine.isRunning())
	{
		Clock.tick();
		Events.pollEvents();
		Engine.update();
	}
	
	return EXIT_SUCCESS;
}