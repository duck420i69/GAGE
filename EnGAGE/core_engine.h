#pragma once

#include "clock.h"
#include "render_system.h"

class ICoreEngine
{
public:
	virtual void init() {};
	virtual void update(float delta) {};
	virtual void shutdown() {};
};

class CoreEngine
{
	std::shared_ptr<ICoreEngine>	mCoreEngine;
	std::shared_ptr<RenderSystem>	mRenderSystem;
	Clock							mClock;
public:
	CoreEngine(std::shared_ptr<ICoreEngine> coreEngine, uint16_t width, uint16_t height, const std::string& title);
	~CoreEngine();

	void run();
};