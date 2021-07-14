#include "pch.h"
#include "core_engine.h"
#include "sandbox_game.h"
#include "logger.h"



int main(void)
{
	
	CoreEngine engine(std::make_shared<SandboxGame>(), 1600, 900, "Hello world");

	engine.run(); 
}