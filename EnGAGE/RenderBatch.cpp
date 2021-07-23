#include "pch.h"
#include "RenderBatch.h"

#include "Globals.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Asset.h"

static constexpr size_t VERTEX_SIZE = 7;

RenderBatch::RenderBatch(uint32_t max_batch_size) noexcept :
	mSprites(), mHasRoom(true),
	mVAO(0), mVBO(0), mEBO(0),
	mMaxBatchSize(max_batch_size),
	mShader(),
	mVertexData(VERTEX_SIZE * 4 * max_batch_size)
{
	mShader = Asset::GetShader("Assets/Shaders/default");

	UploadToGPU();
}

RenderBatch::~RenderBatch() noexcept
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void RenderBatch::AddSprite(const SpriteRenderer& sprite)
{
	mSprites.push_back(sprite);

	const size_t index = mSprites.size() - 1;
	const glm::vec4& color = sprite.GetColor();
	size_t offset = index * 4 * VERTEX_SIZE;

	float x_add = 1.0f;
	float y_add = 1.0f;
	for (uint32_t i = 0; i < 4; i++)
	{
		if (i == 1)
			y_add = 0.0f;
		else if (i == 2)
			x_add = 0.0f;
		else if (i == 3)
			y_add = 1.0f;
		mVertexData[offset] = sprite.mGameObject->mTransform.mPos.x + (x_add * sprite.mGameObject->mTransform.mScale.x);
		mVertexData[offset + 1] = sprite.mGameObject->mTransform.mPos.y + (y_add * sprite.mGameObject->mTransform.mScale.y);
		mVertexData[offset + 2] = 0;

		mVertexData[offset + 3] = color.r;
		mVertexData[offset + 4] = color.g;
		mVertexData[offset + 5] = color.b;
		mVertexData[offset + 6] = color.a;

		offset += VERTEX_SIZE;
	}


	if (mSprites.size() >= mMaxBatchSize)
		mHasRoom = false;
}

void RenderBatch::Render() const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexData.size() * sizeof(float), mVertexData.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mShader->Bind();
	mShader->UploadMat4x4("uProjection", glm::value_ptr(Globals::gCurrentScene->GetCamera().GetProjection()));
	mShader->UploadMat4x4("uView", glm::value_ptr(Globals::gCurrentScene->GetCamera().GetViewMatrix()));

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mSprites.size() * 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glUseProgram(0);
}

void RenderBatch::UploadToGPU() noexcept
{
	std::vector<uint32_t> indices = GenerateIndices();
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_SIZE, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_SIZE, (const void*)(sizeof(float) * 3));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

std::vector<uint32_t> RenderBatch::GenerateIndices() const noexcept
{
	//6 indices per quad
	std::vector<uint32_t> result( (size_t)mMaxBatchSize * 6 );

	for (uint32_t i = 0; i < mMaxBatchSize; i++)
	{
		//3, 2, 0, 0, 2, 1
		const size_t offset = (size_t)i * 6;
		const uint32_t count = i * 4;

		result[offset] = count + 3;
		result[offset + 1] = count + 2;
		result[offset + 2] = count + 0;

		result[offset + 3] = count + 0;
		result[offset + 4] = count + 2;
		result[offset + 5] = count + 1;
	}

	return result;
}

