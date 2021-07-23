#pragma once

#include "Component.h"

#include <glm/vec4.hpp>

class SpriteRenderer : public Component
{
	glm::vec4 mColor;
public:
	SpriteRenderer(const glm::vec4& color) noexcept :
		mColor(color)
	{}

	void Update(double dt) noexcept override {}

	inline const glm::vec4& GetColor() const noexcept { return mColor; }
};