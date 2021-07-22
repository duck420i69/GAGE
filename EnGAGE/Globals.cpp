#include "pch.h"
#include "Globals.h"

uint32_t Globals::gScreenWidth = 1600, Globals::gScreenHeight = 900;

std::string Globals::gScreenTitle = "Hello world";

std::unique_ptr<Scene> Globals::gCurrentScene = nullptr;