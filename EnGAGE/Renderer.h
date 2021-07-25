#pragma once

#include "RenderBatch.h"
#include "SpriteRenderer.h"

#include <vector>
#include <memory>

class Renderer
{
	const uint32_t MAX_BATCH_SIZE = 500;

	std::vector<std::unique_ptr<RenderBatch>> mBatches;
public:
	void Add(const GameObject& go) noexcept
	{
		if (auto ptr = go.GetComponent<SpriteRenderer>().lock())
		{
			Add(ptr);
		}
	}
	void Add(const std::weak_ptr<SpriteRenderer>& sprite) noexcept
	{
		std::shared_ptr sprite_ptr = sprite.lock();
		if (!sprite_ptr) 
			return;

		int z_index = sprite_ptr->mGameObject->ZIndex();
		bool added = false;
		for (auto& batch : mBatches)
		{
			if (batch->HasRoom() && batch->ZIndex() == z_index)
			{
				auto texture = sprite_ptr->GetTexture();
				if (sprite_ptr == nullptr or (batch->HasTexture(texture) or batch->HasTextureRoom()))
				{
					batch->AddSprite(sprite);
					added = true;
					break;
				}
			}
		}

		if (!added)
		{
			auto newBatch = std::make_unique<RenderBatch>(MAX_BATCH_SIZE, z_index);
			newBatch->AddSprite(sprite);
			mBatches.push_back(std::move(newBatch));

		}


		std::sort(mBatches.begin(), mBatches.end(),
			[](const std::unique_ptr<RenderBatch>& a, const std::unique_ptr<RenderBatch>& b)
			{
				return a->ZIndex() < b->ZIndex();
			});
	}

	void Render() const noexcept
	{
		for (const auto& batch : mBatches)
		{
			batch->Render();
		}
	}
	
};