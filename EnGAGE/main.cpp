#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Logger.h"
#include "Globals.h"

#include <GLFW/glfw3.h>

#include "MenuScene.h"

#include "DynamicVertex.h"


int main()
{

	Window::Create(Globals::screen_width, Globals::screen_height, Globals::screen_title);
	Events::Init();
	Opengl::Init();
	Widget::Init();

	Globals::ChangeScene<MenuScene>();

	float start_time = glfwGetTime();
	float end_time = start_time;
	while (!Window::IsCloseRequested())
	{
		end_time = glfwGetTime();
		float dt = end_time - start_time;
		start_time = end_time;

		
	
		Widget::Prepare();

		if (Globals::current_scene) {
			Globals::current_scene->ImGui();
			Globals::current_scene->Update(dt);
			Globals::current_scene->Render();
		}

		Widget::Render();
		Events::Update();
		Window::Update();
	}
	Opengl::Destroy();
	Widget::Destroy();
	Window::Destroy();
}
