#include "pch.h"
#include "Globals.h"

uint32_t Globals::screen_width = 1600, Globals::screen_height = 900;

std::string Globals::screen_title = "Hello world";

std::unique_ptr<Scene> Globals::current_scene = nullptr;

