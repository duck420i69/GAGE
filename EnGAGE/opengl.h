#pragma once

#include "common.h"

class Opengl
{
public:
	static void init();
	static void clear();
private:
	static void messageCallback(U32 source, U32 type, U32 id, U32 severity, U32 length,
		const char* message, const void* userParam);
};