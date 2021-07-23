#pragma once

#include "SpriteRenderer.h"
#include "Shader.h"

/* ===Vertex layout=== */
/* Position(3 floats)    Color(4 floats) */
class RenderBatch
{
	std::vector<SpriteRenderer> mSprites;
	bool						mHasRoom;
	uint32_t					mVAO, mVBO, mEBO;
	uint32_t					mMaxBatchSize;
	std::shared_ptr<Shader>		mShader;
	std::vector<float>			mVertexData;
public:
	RenderBatch(uint32_t max_batch_size) noexcept;
	~RenderBatch() noexcept;

	void AddSprite(const SpriteRenderer& sprite);
	void Render() const noexcept;

	inline bool HasRoom() const noexcept { return mHasRoom; }
private:
	void UploadToGPU() noexcept;
	std::vector<uint32_t> GenerateIndices() const noexcept;
};