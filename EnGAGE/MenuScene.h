#pragma once

#include "Scene.h"

#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include "window.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class MenuScene final : public Scene{
	Model mNanoSuit , mNanoSuit2;
	Camera mCam;
	PointLight mLight;
	
public:
	MenuScene() noexcept :
		mCam(),
		mNanoSuit("Assets/Models/nano_textured/nanosuit.obj"),
		mNanoSuit2("Assets/Models/nano_textured/nanosuit.obj")
	{		
		Opengl::SetProjection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.5f, 100.0f));
	}
	void Update(float delta) noexcept override {
		if (Events::IsKeyDownOnce(Events::KEY_TAB)) {
			Window::ToggleCursor();
		}

		if (!Window::IsCursorEnabled()) {
			if (Events::IsKeyDown(Events::KEY_W)) {
				mCam.MoveForward(delta);
			}
			if (Events::IsKeyDown(Events::KEY_S)) {
				mCam.MoveForward(-delta);
			}
			if (Events::IsKeyDown(Events::KEY_D)) {
				mCam.MoveRight(delta);
			}
			if (Events::IsKeyDown(Events::KEY_A)) {
				mCam.MoveRight(-delta);
			}
			mCam.Rotate(Events::GetDY(), Events::GetDX());
		}
		Opengl::SetCamera(mCam.GetMatrix());
	};
	void Render() noexcept override {
		Opengl::Clear();
		mLight.Bind(mCam.GetMatrix());	
		mNanoSuit.Draw();
		mNanoSuit2.Draw();
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();
		mNanoSuit.RenderTree();
		mNanoSuit2.RenderTree("Model2");

	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
