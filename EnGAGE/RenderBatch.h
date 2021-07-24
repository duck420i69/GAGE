#pragma once

#include "SpriteRenderer.h"
#include "Shader.h"

/* ===Vertex layout=== */
/* Position(3 floats)    Color(4 floats)      TexCoord(2 floats)        TexID(1 float)*/
class RenderBatch
{
	const int VERTEX_SIZE = 10;
	const size_t MAX_TEXTURES = 8;

	std::vector<SpriteRenderer>				mSprites;
	bool									mHasRoom;
	uint32_t								mVAO, mVBO, mEBO;
	uint32_t								mMaxBatchSize;
	std::shared_ptr<Shader>					mShader;
	std::vector<float>						mVertexData;
	std::vector<std::shared_ptr<Texture>>	mTextures;
public:
	RenderBatch(uint32_t max_batch_size) noexcept;
	~RenderBatch() noexcept;

	void AddSprite(const SpriteRenderer& sprite);
	void Render() const noexcept;

	inline bool HasRoom() const noexcept { return mHasRoom; }
	inline bool HasTextureRoom() const noexcept { return mTextures.size() < MAX_TEXTURES; }
	inline bool HasTexture(const std::shared_ptr<Texture>& texture) const noexcept 
	{
		return std::count(mTextures.begin(), mTextures.end(), texture) != 0; 
	}
private:
	void UploadToGPU() noexcept;
	std::vector<uint32_t> GenerateIndices() const noexcept;
};