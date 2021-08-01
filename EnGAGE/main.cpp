#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Logger.h"
#include "Globals.h"
#include "Asset.h"

#include <GLFW/glfw3.h>


int main()
{
	Window::Create(Globals::gScreenWidth, Globals::gScreenHeight, Globals::gScreenTitle);
	Events::Init();
	Opengl::Init();
	Widget::Init();

	double start_time = glfwGetTime();
	double end_time = glfwGetTime();
	while (!Window::IsCloseRequested())
	{
		end_time = glfwGetTime();
		double dt = end_time - start_time;
		start_time = end_time;

		

		Opengl::Clear();
		Widget::Prepare();

		Widget::Render();
		Events::Update();
		Window::Update();
	}
	Widget::Destroy();
	Window::Destroy();
	Asset::Destroy();
}
