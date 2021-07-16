#pragma once

#include "core_engine.h"

class SandboxGame : public ICoreEngine
{
	void init() noexcept override;
	void update(float delta) noexcept override;
	void processCamera(Camera& camera, float delta) noexcept override;
	void shutdown() noexcept override;
};