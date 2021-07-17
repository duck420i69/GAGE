#include "pch.h"
#include "engine.h"
#include "clock.h"
#include "events.h"

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
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
}