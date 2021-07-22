#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Time.h"
#include "LevelEditorScene.h"
#include "LevelScene.h"
#include "Logger.h"
#include "Globals.h"
#include "GameObject.h"

#include "imgui/imgui.h"

class Test : public Component
{
public:
	void Update(double dt) noexcept override {}
};

class Test2 : public Component
{
public:
	void Update(double dt) noexcept override {}
};

int main()
{

	Window::Create(Globals::gScreenWidth, Globals::gScreenHeight, Globals::gScreenTitle);
	Time::Init();
	Events::Init();
	Opengl::Init();
	Widget::Init();

	Globals::ChangeScene<LevelEditorScene>(Globals::gCurrentScene);

	double start_time = Time::GetTime();
	double end_time = Time::GetTime();
	while (!Window::IsCloseRequested())
	{
		end_time = Time::GetTime();
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
}