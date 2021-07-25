#pragma once

#include "SpriteRenderer.h"
#include "Shader.h"

/* ===Vertex layout=== */
/* Position(3 floats)    Color(4 floats)      TexCoord(2 floats)        TexID(1 float)*/
class RenderBatch
{
	const int VERTEX_SIZE = 10;
	const size_t MAX_TEXTURES = 8;

	std::vector<std::weak_ptr<SpriteRenderer>>	mSprites;
	bool										mHasRoom;
	uint32_t									mVAO, mVBO, mEBO;
	uint32_t									mMaxBatchSize;
	std::weak_ptr<Shader>						mShader;
	std::vector<float>							mVertexData;
	std::vector<std::weak_ptr<Texture>>			mTextures;
	int											mZIndex;
public:
	RenderBatch(uint32_t max_batch_size, int z_index) noexcept;
	~RenderBatch() noexcept;

	void AddSprite(const std::weak_ptr<SpriteRenderer>& sprite);
	void Render() noexcept;
	void LoadVertices(int index) noexcept;

	inline bool HasRoom() const noexcept { return mHasRoom; }
	inline bool HasTextureRoom() const noexcept { return mTextures.size() < MAX_TEXTURES; }
	inline bool HasTexture(const std::weak_ptr<Texture>& texture) const noexcept 
	{
		return std::find_if(mTextures.begin(), mTextures.end(), [&](const auto& t)
			{
				return t.lock() == texture.lock();
			}) != mTextures.end();
	}

	inline const int& ZIndex() const noexcept { return mZIndex; }
private:
	void UploadToGPU() noexcept;
	std::vector<uint32_t> GenerateIndices() const noexcept;
};