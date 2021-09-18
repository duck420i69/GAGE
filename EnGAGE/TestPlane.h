#pragma once

#include "Drawable.h"

class TestPlane final : public Drawable {
	std::vector<std::shared_ptr<Bindable>> mHaloBindables;
	glm::vec3 mPos = {0, 0, 0};
public:
	TestPlane();
	void SpawmControlWindow(const char* name) noexcept;
	glm::mat4x4 GetTransform() const noexcept override;
};