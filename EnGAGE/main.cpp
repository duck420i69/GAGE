#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"
#include "Time.h"
#include "LevelEditorScene.h"
#include "LevelScene.h"
#include "Logger.h"

#include "imgui/imgui.h"

static std::unique_ptr<IScene> g_current_scene = nullptr;

template<typename T>
static void ChangeScene(std::unique_ptr<IScene>& current_scene) noexcept
{
	if (current_scene)
	{
		if (typeid(*current_scene.get()).name() == typeid(T).name())
		{
			return;
		}
		current_scene.reset();
	}

	Logger::info("Switching current scene to: {}", typeid(T).name());
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

		if (Events::IsKeyDownOnce(Events::KEY_1))
		{
			ChangeScene<LevelEditorScene>(g_current_scene);
		}
		else if (Events::IsKeyDownOnce(Events::KEY_2))
		{
			ChangeScene<LevelScene>(g_current_scene);
		}

		Opengl::Clear();
		Widget::Prepare();

		if (g_current_scene)
		{
			g_current_scene->Update(dt);
			g_current_scene->Render();
		}

		Widget::Render();
		Events::Update();
		Window::Update();
	}

	if (g_current_scene)
		g_current_scene.reset();
	Widget::Destroy();
	Window::Destroy();
}