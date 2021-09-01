#pragma once

#include "Scene.h"

#include "Box.h"
#include "Sheet.h"
#include "Camera.h"

#include <imgui.h>

class MenuScene final : public Scene{
	std::vector<std::unique_ptr<Drawable>> mDrawables;
	Camera mCam;
public:
	MenuScene() noexcept :
		mCam(1, 0, 0)
	{
		std::mt19937 rng(std::random_device{} ());
		std::uniform_real_distribution<float> a(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> d(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> o(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> r(6.0f, 24.0f);

		for (unsigned int i = 0; i < 40; i++) {
			mDrawables.push_back(std::make_unique<Box>(rng, a, d, o ,r));
		}
		for (unsigned int i = 0; i < 40; i++) {
			mDrawables.push_back(std::make_unique<Sheet>(rng, a, d, o, r));
		}
		
		Opengl::SetProjection(glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f));
	}
	void Update(float delta) noexcept override {
		Opengl::SetCamera(mCam.GetMatrix());
		for(auto& drawable : mDrawables)
			drawable->Update(delta);
	};
	void Render() noexcept override {
		Opengl::Clear();
		for (const auto& drawable : mDrawables)
			drawable->Draw();
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
