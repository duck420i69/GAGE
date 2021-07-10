#pragma once

#include "core_engine.h"

class SandboxGame : public ICoreEngine
{
	void init() override;
	void update(Real delta) override;
	void render() override;
	void shutdown() override;
};