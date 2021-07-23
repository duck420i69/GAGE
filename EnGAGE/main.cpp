#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "LevelEditorScene.h"
#include "LevelScene.h"
#include "Logger.h"
#include "Globals.h"
#include "GameObject.h"
#include "Asset.h"

#include <GLFW/glfw3.h>


int main()
{

	Window::Create(Globals::gScreenWidth, Globals::gScreenHeight, Globals::gScreenTitle);
	Events::Init();
	Opengl::Init();
	Widget::Init();

	Globals::ChangeScene<LevelEditorScene>(Globals::gCurrentScene);

	double start_time = glfwGetTime();
	double end_time = glfwGetTime();
	while (!Window::IsCloseRequested())
	{
		end_time = glfwGetTime();
		double dt = end_time - start_time;
		start_time = end_time;

		if (Events::IsKeyDownOnce(Events::KEY_1))
		{
			Globals::ChangeScene<LevelEditorScene>(Globals::gCurrentScene);
		}
		else if (Events::IsKeyDownOnce(Events::KEY_2))
		{
			Globals::ChangeScene<LevelScene>(Globals::gCurrentScene);
		}

		Opengl::Clear();
		Widget::Prepare();

		if (Globals::gCurrentScene)
		{
			Globals::gCurrentScene->Update(dt);
			Globals::gCurrentScene->Render();
		}

		Widget::Render();
		Events::Update();
		Window::Update();
	}

	if (Globals::gCurrentScene)
		Globals::gCurrentScene.reset();
	Widget::Destroy();
	Window::Destroy();
	Asset::Destroy();
}