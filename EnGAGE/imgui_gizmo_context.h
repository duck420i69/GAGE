#pragma once

#include "camera.h"

class ImGuiGizmoContext
{
public:
	static void init() noexcept;
	static void prepare(const Camera& camera) noexcept;
};