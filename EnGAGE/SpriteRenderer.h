#pragma once

#include "Component.h"
#include "Texture.h"
#include "Sprite.h"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

class SpriteRenderer : public Component
{
	glm::vec4 mColor;
	Sprite    mSprite;
public:
	SpriteRenderer(const glm::vec4& color) noexcept :
		mColor(color), mSprite(nullptr)
	{}

	SpriteRenderer(const Sprite& sprite) noexcept :
		mColor({ 1, 1, 1, 1 }), mSprite(sprite)
	{}


	void Update(double dt) noexcept override {}

	inline const glm::vec4& GetColor() const noexcept { return mColor; }
	inline const std::shared_ptr<Texture>& GetTexture() const noexcept { return mSprite.GetTexture(); }
	inline const std::array<glm::vec2, 4>& GetTexCoords() const noexcept { return mSprite.GetTexCoords(); }
};