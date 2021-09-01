#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>

#include "Logger.h"

class Camera {
	mutable glm::mat4 mMatrix;
	float r, a, b;
public:
	Camera(float r, float a, float b) noexcept :
		r(r), a(a), b(b) {}

	void SpawnControlWindow() noexcept {
		ImGui::Begin("Camera");
		ImGui::DragFloat("R", &r);
		ImGui::DragFloat("A", &a);
		ImGui::DragFloat("B", &b);

		if (ImGui::Button("Reset")) {
			r = 1;
			a = 0;
			b = 0;
		}

		ImGui::End();

		if (b > 89) b = 89;
		else if (b < -89) b = -89;

		if (a > 360) a = -359;
		else if (a < -360) a = 359;
	}

	const glm::mat4& GetMatrix() const noexcept{

		glm::vec3 pos = glm::vec3{ r, 0.0f, 0.0f };
		pos = glm::rotate(pos, glm::radians(b), { 0, 0, 1 });
		pos = glm::rotate(pos, glm::radians(a), { 0, 1, 0 });


		mMatrix = glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		return mMatrix;
	}
};