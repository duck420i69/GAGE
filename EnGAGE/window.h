#pragma once

struct GLFWwindow;
class Window
{
	friend class Events;

	static uint32_t sWidth, sHeight;
	static GLFWwindow* sWindow;
	static std::string sTitle;
	static bool sCursorEnabled;
public:
	static void Create(uint32_t width, uint32_t height, const std::string& title) noexcept;
	static void Destroy() noexcept;

	static bool IsCloseRequested() noexcept;
	static void Update() noexcept;

	static const uint32_t& GetWidth() noexcept;
	static const uint32_t& GetHeight() noexcept;

	static void ToggleCursor() noexcept;
	static void EnableCursor() noexcept;
	static void DisableCursor() noexcept;
	static bool IsCursorEnabled() noexcept;

	static GLFWwindow* GetWindow() noexcept;
};