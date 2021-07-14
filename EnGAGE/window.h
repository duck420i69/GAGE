#pragma once

#include <GLFW/glfw3.h>

/* Quản lý cửa sổ (Window) */
class Window
{
	static GLFWwindow*	sWindow;
	static uint16_t		sWidth, sHeight;
	static std::string	sTitle;
public:
	/* Khởi tạo cửa sổ */
	static void createWindow(uint16_t width, uint16_t height, const std::string& title) noexcept;

	static bool shouldClose() noexcept;
	static void swapBuffers() noexcept;
	static void pollEvents() noexcept;

	static GLFWwindow* getWindow();

	/* Tăt của sổ */
	static void shutdown() noexcept;
};
