#include "pch.h"
#include "core_engine.h"

#include "window.h"
#include "opengl.h"
#include "imgui_context.h"
#include "ecs.h"

#include "components.h"

CoreEngine::CoreEngine(std::shared_ptr<ICoreEngine> coreEngine, uint16_t width, uint16_t height, const std::string&  title) :
	mCoreEngine(coreEngine), mClock()
{
	Logger::info("Engine created !");
	Window::createWindow(width, height, title);
	Opengl::init();
	ImguiContext::init(Window::getWindow());

	//Khởi động entity component system
	Signature signature;
	ECS& ecs = ECS::getInstance();
	ecs.init();
	ecs.registerComponent<TransformComponent>();
	ecs.registerComponent<MeshComponent>();

	mRenderSystem = ecs.registerSystem<RenderSystem>();

	signature.reset();
	signature.set(ecs.getComponentType<TransformComponent>(), true);
	signature.set(ecs.getComponentType<MeshComponent>(), true);
	ecs.setSystemSignature<RenderSystem>(signature);
	

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
		mCoreEngine->update(mClock.getDelta());


		Opengl::clear();
		ImguiContext::prepare();

		mRenderSystem->render();

		ImguiContext::render();
		Window::swapBuffers();

	}
}
