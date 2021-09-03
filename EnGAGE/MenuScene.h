#pragma once

#include "Scene.h"

#include "Box.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

#include <imgui.h>

class MenuScene final : public Scene{
	std::vector<std::unique_ptr<Drawable>> mDrawables;
	Camera mCam;
	PointLight mLight;
public:
	MenuScene() noexcept :
		mCam(1, 0, 0)
	{
		std::mt19937 rng(std::random_device{} ());
		std::uniform_real_distribution<float> a(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> d(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> o(0.0f, 3.14f / 6.0f);
		std::uniform_real_distribution<float> c(0.0f, 1.0f);
		std::uniform_real_distribution<float> r(6.0f, 24.0f);

		for (unsigned int i = 0; i < 1500; i++) {
			mDrawables.push_back(std::make_unique<Box>(rng, a, d, o ,r, glm::vec3(c(rng), c(rng), c(rng))));
		}

		for (unsigned int i = 0; i < 1500; i++) {
			mDrawables.push_back(std::make_unique<Model>(rng, a, d, o, r, glm::vec3(c(rng), c(rng), c(rng))));
		}
		
		
		Opengl::SetProjection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.5f, 100.0f));
	}
	void Update(float delta) noexcept override {
		Opengl::SetCamera(mCam.GetMatrix());
		for(auto& drawable : mDrawables)
			drawable->Update(delta);
	};
	void Render() noexcept override {
		Opengl::Clear();
		mLight.Bind(Opengl::GetCamera());
		for (const auto& drawable : mDrawables)
			drawable->Draw();
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();
	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
