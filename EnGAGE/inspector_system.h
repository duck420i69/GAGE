#pragma once
#include "system.h"

class InspectorSystem : public System
{
public:
	void renderImGui() const noexcept;
};