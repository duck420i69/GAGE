#pragma once

#include "clock.h"
#include "render_system.h"
#include "inspector_system.h"
#include "camera.h"

class ICoreEngine
{
public:
	virtual void init() noexcept {};
	virtual void update(float delta) noexcept {};
	virtual void processCamera(Camera& camera, float delta) noexcept {};
	virtual void shutdown() noexcept {};
};

class CoreEngine
{
	std::shared_ptr<ICoreEngine>		mCoreEngine;
	std::shared_ptr<RenderSystem>		mRenderSystem;
	std::shared_ptr<InspectorSystem>	mInspectorSystem;
	Camera								mCamera;
	Clock								mClock;
	bool								mSpawnImGuiWindow;
	
public:
	CoreEngine(std::shared_ptr<ICoreEngine> coreEngine, uint16_t width, uint16_t height, const std::string& title) noexcept;
	~CoreEngine() noexcept;

	void run() noexcept;
private:
	void spawnImGuiWindows(float delta) noexcept;
};