#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>

#include "Logger.h"

class Camera {
	glm::vec3 mPos, mForward, mRight;
	float mPitch, mYaw;
	float mSpeed;

	static constexpr float SENSITIVITY = 0.4f;
	static constexpr float FLY_SPEED = 7.0f;
public:
	Camera() noexcept  {
		Reset();
	}

	void Reset() noexcept {
		mRight = { 1, 0, 0 };
		mForward = {0, 0, -1};
		mPos = { 0, 0, 0 };
		mPitch = 0.0f;
		mYaw = 0.0f;
		mSpeed = FLY_SPEED;
	}
	void Rotate(float dPitch, float dYaw) noexcept {


		mYaw += -dYaw * SENSITIVITY;
		mPitch += -dPitch * SENSITIVITY;

		wrap_angle(mYaw);
		clamp_angle(mPitch, -89.0f, 89.0f);


	}

	void MoveForward(float delta) noexcept {
		mPos += mForward * mSpeed * delta;
	}

	void MoveRight(float delta) noexcept {
		mPos += mRight * mSpeed * delta;
	}

	void SpawnControlWindow() noexcept {
		ImGui::Begin("Camera");
		ImGui::DragFloat3("Position", &mPos.x, 0.1f);
		ImGui::DragFloat("Pitch", &mPitch);
		ImGui::DragFloat("Yaw", &mYaw);
		ImGui::DragFloat("Fly Speed", &mSpeed, 0.1f, 1.0f, 10.0f);

		if (ImGui::Button("Reset")) {
			Reset();
		}

		ImGui::End();
	}

	const glm::mat4 GetMatrix() noexcept{
		static constexpr glm::vec3 up = glm::vec3(0, 1, 0);
		static constexpr glm::vec3 default_forward = glm::vec3(0, 0, -1);
		mForward = glm::rotate(default_forward, glm::radians(mYaw), up);
		mRight = glm::cross(mForward, up);
		mForward = glm::rotate(mForward, glm::radians(mPitch), mRight);

		return glm::lookAt(mPos, mPos + mForward, glm::vec3(0, 1, 0));
	}
private:
	void wrap_angle(float& angle) {
		if (angle > 360) angle = -359;
		else if (angle < -360) angle = 359;
	}
	void clamp_angle(float& angle, const float min, const float max) {
		angle = std::clamp(angle, min, max);
	}
};