#pragma once


class Opengl
{
public:
	static void init();
	static void clear();
private:
	static void messageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, uint32_t length,
		const char* message, const void* userParam);
};