#pragma once

#include "Drawable.h"

class Mesh : public Drawable {
	glm::mat4 transform;
public:
	Mesh(const class Material& material, const struct aiMesh& mesh,
		const std::filesystem::path& full_path, float scale) noexcept;

	inline void SetTransform(const glm::mat4x4& transform) noexcept { this->transform = transform; }
	inline glm::mat4 GetTransform() const noexcept override {
		return transform;
	}
};

