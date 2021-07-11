#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "pch.h"

/* Quản lý cửa sổ (Window) */
class Window
{
	static GLFWwindow*	sWindow;
	static U16			sWidth, sHeight;
	static String		sTitle;
public:
	/* Khởi tạo cửa sổ */
	static void createWindow(U16 width, U16 height, StringRef title) noexcept;

	static bool shouldClose() noexcept;
	static void swapBuffers() noexcept;
	static void pollEvents() noexcept;

	/* Tăt của sổ */
	static void shutdown() noexcept;
};
