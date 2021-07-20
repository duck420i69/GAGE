#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Time.h"
#include "LevelEditorScene.h"
#include "LevelScene.h"

#include "imgui/imgui.h"

static std::unique_ptr<IScene> g_current_scene = nullptr;

template<typename T>
static void ChangeScene(std::unique_ptr<IScene>& current_scene) noexcept
{
	if (current_scene)
		current_scene.reset();
	
	current_scene = std::make_unique<T>();
}

int main()
{
	Window::Create(1600, 900, "Hello world");
	Time::Init();
	Events::Init();
	Opengl::Init();
	Widget::Init();

	ChangeScene<LevelEditorScene>(g_current_scene);
	
	double start_time = Time::GetTime();
	double end_time = Time::GetTime();
	while (!Window::IsCloseRequested())
	{
		end_time = Time::GetTime();
		double dt = end_time - start_time;
		start_time = end_time;

		Opengl::Clear();
		Widget::Prepare();


		Widget::Render();
		Events::Update();
		Window::Update();

		

		if (Events::IsKeyDown(Events::KEY_1))
		{
			ChangeScene<LevelEditorScene>(g_current_scene);
		}
		else if (Events::IsKeyDown(Events::KEY_2))
		{
			ChangeScene<LevelScene>(g_current_scene);
		}

		if (g_current_scene)
			g_current_scene->Update(dt);
	}

	if (g_current_scene)
		g_current_scene.reset();
	Widget::Destroy();
	Window::Destroy();
}