#include "pch.h"
#include "core_engine.h"

#include "window.h"
#include "input.h"
#include "opengl.h"
#include "imgui_context.h"
#include "imgui_gizmo_context.h"
#include "ecs.h"
#include "performance_report.h"
#include "loader.h"
#include "shader.h"

#include "components.h"


CoreEngine::CoreEngine(std::shared_ptr<ICoreEngine> coreEngine, uint16_t width, uint16_t height, const std::string& title) noexcept  :
	mCoreEngine(coreEngine), mCamera(), mClock(), mSpawnImGuiWindow(true)
{
	Logger::init();
	Logger::info("Engine created !");

	Window::createWindow(width, height, title);
	Opengl::init();
	ImguiContext::init(Window::getWindow());
	Input::init(Window::getWindow());
	PerformanceReport::init();

	//Khởi động entity component system
	Signature signature;
	ECS& ecs = ECS::getInstance();
	ecs.init();
	ecs.registerComponent<TransformComponent>();
	ecs.registerComponent<MeshComponent>();
	ecs.registerComponent<NameComponent>();

	mRenderSystem = ecs.registerSystem<RenderSystem>();
	mInspectorSystem = ecs.registerSystem<InspectorSystem>();

	signature.reset();
	signature.set(ecs.getComponentType<TransformComponent>(), true);
	signature.set(ecs.getComponentType<MeshComponent>(), true);
	ecs.setSystemSignature<RenderSystem>(signature);

	signature.reset();
	signature.set(ecs.getComponentType<NameComponent>(), true);
	ecs.setSystemSignature<InspectorSystem>(signature);

	mCoreEngine->init();
}

CoreEngine::~CoreEngine() noexcept
{
	mCoreEngine->shutdown();
	ImguiContext::shutdown();
	Shader::shutdown();
	Loader::shutdown();
	Window::shutdown();
	Logger::info("Engine shutting down !");
}

void CoreEngine::run() noexcept
{

	while (!Window::shouldClose())
	{
		Input::update();
		Window::pollEvents();
		mClock.tick();
		const float& delta = mClock.getDelta();
		mCoreEngine->update(delta);
		mCoreEngine->processCamera(mCamera, delta);
		mCamera.buildMatrices();

		Opengl::clear();
		ImguiContext::prepare();
		ImGuiGizmoContext::prepare(mCamera);

		mRenderSystem->render(mCamera);

		spawnImGuiWindows(delta);

		ImguiContext::render();
		Window::swapBuffers();

	}
}

void CoreEngine::spawnImGuiWindows(float delta) noexcept
{
	if (mSpawnImGuiWindow)
	{
		mInspectorSystem->renderImGui();
		Logger::renderImGui();
		PerformanceReport::reportToImGui(delta);
	}
	if (Input::isKeyDownOnce(Input::KEY_F1))
	{
		mSpawnImGuiWindow = !mSpawnImGuiWindow;
	}
}
