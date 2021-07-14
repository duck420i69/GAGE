#pragma once

#include "core_engine.h"

class SandboxGame : public ICoreEngine
{
	void init() override;
	void update(float delta) override;
	void shutdown() override;
};