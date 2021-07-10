#include "pch.h"

#include "window.h"
#include "core_engine.h"

CoreEngine::CoreEngine(SharedRef<ICoreEngine> coreEngine, U16 width, U16 height, StringRef title) :
	mCoreEngine(coreEngine)
{
	Logger::info("Engine created !");
	Window::createWindow(width, height, title);
	mCoreEngine->init();
}

CoreEngine::~CoreEngine()
{
	mCoreEngine->shutdown();
	Window::shutdown();
	Logger::info("Engine shutting down !");
}

void CoreEngine::run()
{
	while (!Window::shouldClose())
	{
		mCoreEngine->update(0.0f);
		mCoreEngine->render();
		Window::swapBuffers();
		Window::pollEvents();
	}
}
