#include "pch.h"
#include "core_engine.h"
#include "sandbox_game.h"

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	CoreEngine engine(std::make_shared<SandboxGame>(), 1600, 900, "Hello world");
	engine.run();
}