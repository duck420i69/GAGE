#include "pch.h"

#include "Window.h"

int main()
{
	Window::Create(1600, 900, "Hello world");

	while (!Window::IsCloseRequested())
	{

		Window::Update();
	}

	Window::Destroy();
}