#pragma once

#include "DrawableBase.h"

#include <glm/gtc/matrix_transform.hpp>

template<typename T>
class OrbitDrawable : public DrawableBase<T> {
	float pitch = 0, yaw = 0, roll = 0;
	float pitch2 = 0, yaw2 = 0, roll2 = 0;
	float r;
	float dpitch, dyaw, droll;
	float dpitch2, dyaw2, droll2;
public:
	OrbitDrawable(std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& d,
		std::uniform_real_distribution<float>& o,
		std::uniform_real_distribution<float>& r) noexcept :
		dpitch(a(rng)), dyaw(a(rng)), droll(a(rng)),
		dpitch2(d(rng)), dyaw2(d(rng)), droll2(d(rng)),
		r(r(rng)) {}

	inline void Update(float dt) noexcept override {
		pitch += dpitch * dt;
		pitch2 += dpitch2 * dt;
		yaw += dyaw * dt;
		yaw2 += dyaw2 * dt;
		roll += droll * dt;
		roll2 += droll2 * dt;
	};

	inline glm::mat4x4 GetTransform() const noexcept override {
		glm::mat4x4 result(1.0f);

		result = glm::rotate(result, roll2, { 0, 0, 1 });
		result = glm::rotate(result, yaw2, { 0, 1, 0 });
		result = glm::rotate(result, pitch2, { 1, 0, 0 });

		result = glm::translate(result, { r, 0, 0 });

		result = glm::rotate(result, roll, { 0, 0, 1 });
		result = glm::rotate(result, yaw, { 0, 1, 0 });
		result = glm::rotate(result, pitch, { 1, 0, 0 });


		return result;
	};
};