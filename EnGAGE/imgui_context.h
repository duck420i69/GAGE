#pragma once


struct GLFWwindow;
class ImguiContext
{
public:
	static void init(GLFWwindow* window) noexcept;
	static void prepare() noexcept;
	static void render() noexcept;
	static void shutdown() noexcept;
};