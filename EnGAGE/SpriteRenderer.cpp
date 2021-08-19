#include "pch.h"
#include "SpriteRenderer.h"

#include "Asset.h"
#include "Logger.h"
#include "Window.h"
#include "Camera.h"
#include "TileType.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>


SpriteRenderer::SpriteRenderer() noexcept :
	mShader(Asset::GetShader("Assets/Shaders/tile"))
{
	const float vertex_data[] = {
		0, 1, 0, 0,
		0, 0, 0, 1,
		1, 1, 1, 0,
		1, 1, 1, 0,
		0, 0, 0, 1,
		1, 0, 1, 1
	};


	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Logger::info("Loading tilemap's gpu data...");
}

SpriteRenderer::~SpriteRenderer() noexcept
{
	Logger::info("Deleting tilemap...");
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	mVAO = 0;
	mVBO = 0;
}

void SpriteRenderer::Update(const Camera& player) noexcept
{
	//Update matrices
	float aspectRatio = (float)Window::GetWidth() / (float)Window::GetHeight();
	float zoom = player.GetZoom();
	auto pos = player.GetPos();
	mProjection = glm::ortho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom);
	mView = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0));
}

void SpriteRenderer::Prepare() const noexcept
{
	auto shader = mShader.lock();
	shader->Bind();
	glBindVertexArray(mVAO);

	shader->UploadFloat("uAlpha", 0.0f);
	shader->UploadMat4x4("uProjection", glm::value_ptr(mProjection));
	shader->UploadMat4x4("uView", glm::value_ptr(mView));
}


void SpriteRenderer::Render(const std::vector<Enemy>& enemies) const noexcept
{
	auto shader = mShader.lock();
	glm::mat4 model;
	for (const auto& e : enemies) {
		model = glm::translate(glm::mat4(1.0f), { e.GetX(), e.GetY(), 0 });
		shader->UploadMat4x4("uModel", glm::value_ptr(model));
		e.GetTexture().lock()->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
}

void SpriteRenderer::Render(unsigned int width, unsigned int height, const std::vector<Tile>& tiles) const noexcept
{
	auto shader = mShader.lock();

	glm::mat4 model;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			const auto& tile = tiles[x + y * width];
			if (tile.type == (unsigned int)TileType::NONE || tile.type == (unsigned int)LogicTileType::NONE)
				continue;
			model = glm::translate(glm::mat4(1.0f), { x, y, 0 });
			shader->UploadMat4x4("uModel", glm::value_ptr(model));
			tile.texture.lock()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}


void SpriteRenderer::Render(const std::vector<std::unique_ptr<Tower>>& towers)
{
	auto shader = mShader.lock();

	for (const auto& tower : towers) {
		if (!tower) continue;
		const auto& t = *tower;
		auto base_texture = t.GetBaseTexture().lock();
		auto cannon_texture = t.GetCannonTexture().lock();
		glm::mat4 model;
		model = glm::translate(glm::mat4(1.0f), { t.GetPos().x, t.GetPos().y, 0 });
		shader->UploadMat4x4("uModel", glm::value_ptr(model));

		//Render base first
		base_texture->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Update the matrix to rotate the cannon first
		model = glm::translate(glm::mat4(1.0f), { t.GetPos().x + 0.5f, t.GetPos().y + 0.5f, 0 });
		model = glm::rotate(model, glm::radians(t.GetRotation()), { 0, 0, 1 });
		model = glm::translate(model, { -0.5f, -0.5f, 0 });

		shader->UploadMat4x4("uModel", glm::value_ptr(model));

		//Render cannon second
		cannon_texture->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void SpriteRenderer::EndRender() const noexcept
{
	glBindVertexArray(0);
	glUseProgram(0);
}


void SpriteRenderer::RenderOpaque(unsigned int x, unsigned int y, const std::weak_ptr<Texture>& texture) {
	auto shader = mShader.lock();
	glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), { x, y, 0 });
	shader->UploadMat4x4("uModel", glm::value_ptr(model));
	shader->UploadFloat("uAlpha", 0.5f);
	texture.lock()->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
