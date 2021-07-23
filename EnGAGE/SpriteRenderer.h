#pragma once

#include "Component.h"
#include "Texture.h"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

class SpriteRenderer : public Component
{
	glm::vec4 mColor;
	std::array<glm::vec2, 4> mTexCoords;
	std::shared_ptr<Texture> mTexture;
public:
	SpriteRenderer(const glm::vec4& color) noexcept :
		mColor(color), mTexture(nullptr)
	{
		GenUVs();
	}

	SpriteRenderer(const std::shared_ptr<Texture>& texture) noexcept :
		mTexture(texture), mColor({ 1, 1, 1, 1 })
	{
		GenUVs();
	}


	void Update(double dt) noexcept override {}

	inline const glm::vec4& GetColor() const noexcept { return mColor; }
	inline const std::shared_ptr<Texture>& GetTexture() const noexcept { return mTexture; }
	inline const std::array<glm::vec2, 4>& GetTexCoords() const noexcept { return mTexCoords; }


private:
	//Will remove
	void GenUVs() noexcept
	{
		mTexCoords[0] = { 1, 1 };
		mTexCoords[1] = { 1, 0 };
		mTexCoords[2] = { 0, 0 };
		mTexCoords[3] = { 0, 1 };
	}
};