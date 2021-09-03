#pragma once

#include "Scene.h"

#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class MenuScene final : public Scene{
	Model mNanoSuit;
	Camera mCam;
	PointLight mLight;
	
public:
	MenuScene() noexcept :
		mCam(1, 0, 0),
		mNanoSuit("Assets/Models/nano_suit.obj")
	{		
		Opengl::SetProjection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.5f, 100.0f));
	}
	void Update(float delta) noexcept override {
		Opengl::SetCamera(mCam.GetMatrix());
	};
	void Render() noexcept override {
		Opengl::Clear();
		mLight.Bind(mCam.GetMatrix());	
		mNanoSuit.Draw();
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();
		mNanoSuit.RenderTree();
	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
