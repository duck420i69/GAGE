#pragma once

#include "Scene.h"

#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include "window.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class MenuScene final : public Scene{
	Model mNanoSuit;
	Camera mCam;
	PointLight mLight;
	
public:
	MenuScene() noexcept :
		mCam(),
		mNanoSuit("Assets/Models/nano_suit.obj")
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
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();
		mNanoSuit.RenderTree();

	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
