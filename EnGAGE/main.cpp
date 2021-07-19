#include "pch.h"

#include "Window.h"
#include "Opengl.h"
#include "Widget.h"
#include "Events.h"

#include "imgui/imgui.h"

int main()
{
	Window::Create(1600, 900, "Hello world");
	Events::Init();
	Opengl::Init();
	Widget::Init();

	while (!Window::IsCloseRequested())
	{
		Opengl::Clear();
		Widget::Prepare();

		ImGui::Text("left: %i, right: %i, middle: %i",
			Events::IsButtonDown(Events::MOUSE_BUTTON_LEFT), 
			Events::IsButtonDown(Events::MOUSE_BUTTON_RIGHT),
			Events::IsButtonDown(Events::MOUSE_BUTTON_MIDDLE));
		ImGui::Text("MouseX: %f, MouseY: %f", Events::GetX(), Events::GetY());
		ImGui::Text("MouseDX: %f, MouseDY: %f", Events::GetDX(), Events::GetDY());
		ImGui::Text("Dragging: %i", Events::IsDragging());

		if (Events::IsKeyDown(Events::))
		{

		}

		Widget::Render();
		Events::Update();
		Window::Update();
	}

	Widget::Destroy();
	Window::Destroy();
}