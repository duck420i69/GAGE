#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"

#include "imgui/imgui.h"

int main()
{
	Window::Create(1600, 900, "Hello world");
	Opengl::Init();
	Widget::Init();

	while (!Window::IsCloseRequested())
	{
		Opengl::Clear();
		Widget::Prepare();

		ImGui::Text("Hello world");

		Widget::Render();
		Window::Update();
	}

	Widget::Destroy();
	Window::Destroy();
}