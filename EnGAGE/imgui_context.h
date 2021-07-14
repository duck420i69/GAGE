#pragma once


struct GLFWwindow;
class ImguiContext
{
public:
	static void init(GLFWwindow* window);
	static void prepare();
	static void render();
	static void shutdown();
};