#include "pch.h"

#include "Window.h"
#include "Opengl.h"

int main()
{
	Window::Create(1600, 900, "Hello world");
	Opengl::Init();
	while (!Window::IsCloseRequested())
	{
		Opengl::Clear();

		Window::Update();
	}

	Window::Destroy();
}