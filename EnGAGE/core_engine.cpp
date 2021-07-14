#include "pch.h"

#include "window.h"
#include "core_engine.h"
#include "opengl.h"


CoreEngine::CoreEngine(SharedRef<ICoreEngine> coreEngine, U16 width, U16 height, StringRef title) :
	mCoreEngine(coreEngine), mClock()
{
	Logger::info("Engine created !");
	Window::createWindow(width, height, title);
	Opengl::init();
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
		mClock.tick();
		Opengl::clear();
		mCoreEngine->update(mClock.getDelta());
		mCoreEngine->render();
		Window::swapBuffers();
		Window::pollEvents();

	}
}
