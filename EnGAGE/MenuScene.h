#pragma once

#include "Scene.h"

#include "Box.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class MenuScene final : public Scene{
	Model mNanoSuit;
	Camera mCam;
	PointLight mLight;
	struct {
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
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

		glm::mat4 model;
		model = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, pos.z });
		model = glm::rotate(model, glm::radians(pos.pitch), {1, 0, 0});
		model = glm::rotate(model, glm::radians(pos.yaw), {0, 1, 0});
		model = glm::rotate(model, glm::radians(pos.roll), {0, 0, 1});
		mNanoSuit.Draw(model);
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();

		ImGui::Begin("Nano suit");
		ImGui::DragFloat3("Rotation", &pos.roll);
		ImGui::DragFloat3("Position", &pos.x);
		ImGui::End();

	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
