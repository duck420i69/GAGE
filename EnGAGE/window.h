#pragma once

struct GLFWwindow;
class Window
{
	static uint32_t sWidth, sHeight;
	static GLFWwindow* sWindow;
	static std::string sTitle;
public:
	static void Create(uint32_t width, uint32_t height, const std::string& title) noexcept;
	static void Destroy() noexcept;

	static bool IsCloseRequested() noexcept;
	static void Update() noexcept;
};