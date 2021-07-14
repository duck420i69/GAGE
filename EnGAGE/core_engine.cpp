#include "pch.h"

#include "window.h"
#include "core_engine.h"
#include "opengl.h"
#include "imgui_context.h"

CoreEngine::CoreEngine(SharedRef<ICoreEngine> coreEngine, U16 width, U16 height, StringRef title) :
	mCoreEngine(coreEngine), mClock()
{
	Logger::info("Engine created !");
	Window::createWindow(width, height, title);
	Opengl::init();
	ImguiContext::init(Window::getWindow());
	mCoreEngine->init();
}

CoreEngine::~CoreEngine()
{
	mCoreEngine->shutdown();
	ImguiContext::shutdown();
	Window::shutdown();
	Logger::info("Engine shutting down !");
}

void CoreEngine::run()
{

	while (!Window::shouldClose())
	{
		Window::pollEvents();
		mClock.tick();


		Opengl::clear();
		ImguiContext::prepare();

		mCoreEngine->update(mClock.getDelta());
		mCoreEngine->render();

		ImguiContext::render();
		Window::swapBuffers();

	}
}
