#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Logger.h"
#include "Globals.h"
#include "Asset.h"

#include "TileType.h"
#include "MenuScene.h"



#include <GLFW/glfw3.h>


int main()
{
	Window::Create(Globals::gScreenWidth, Globals::gScreenHeight, Globals::gScreenTitle);
	Events::Init();
	Opengl::Init();
	Widget::Init();

	{
		TileType::Load();
		Globals::ChangeScene<MenuScene>();

		double start_time = glfwGetTime();
		double end_time = glfwGetTime();
		while (!Window::IsCloseRequested())
		{
			end_time = glfwGetTime();
			float dt = (float)(end_time - start_time);
			start_time = end_time;

			if (dt > 0.05f) {
				dt = 0.05f;
			}

			Globals::gCurrentScene->Update(dt);

			Opengl::Clear();
			Widget::Prepare();


			Globals::gCurrentScene->ImGui();
			Globals::gCurrentScene->Render();

			Widget::Render();
			Events::Update();
			Window::Update();
		}

		if (Globals::gCurrentScene) {
			Globals::gCurrentScene.reset();
		}
	}
	Widget::Destroy();
	Window::Destroy();
	Asset::Destroy();
}
