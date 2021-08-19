#pragma once

#include "Shader.h"
#include "Tile.h"
#include "Enemy.h"
#include "Tower.h"

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

class Camera;
class SpriteRenderer {
	unsigned int mVAO = 0, mVBO = 0;
	std::weak_ptr<Shader> mShader;

	glm::mat4	mProjection = glm::mat4(1.0f);
	glm::mat4	mView = glm::mat4(1.0f);
public:
	SpriteRenderer() noexcept;
	~SpriteRenderer() noexcept;

	void Update(const Camera& player) noexcept;
	void Prepare() const noexcept;
	void Render(const std::vector<Enemy>& enemies) const noexcept;
	void Render(unsigned int width, unsigned int height, const std::vector<std::unique_ptr<Tile>>& tiles) const noexcept;
	void Render(const std::vector<std::unique_ptr<Tower>>& towers);
	void EndRender() const noexcept;

	void RenderOpaque(unsigned int x, unsigned int y, const std::weak_ptr<Texture>& texture);
};