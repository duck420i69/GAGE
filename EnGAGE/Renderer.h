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
			Add(*ptr);
		}
	}
	void Add(const SpriteRenderer& sprite) noexcept
	{
		bool added = false;
		for (auto& batch : mBatches)
		{
			if (batch->HasRoom())
			{
				batch->AddSprite(sprite);
				added = true;
				break;
			}
		}

		if (!added) 
		{
			auto newBatch = std::make_unique<RenderBatch>(MAX_BATCH_SIZE);
			newBatch->AddSprite(sprite);
			mBatches.push_back(std::move(newBatch));

		}
	}

	void Render() noexcept
	{
		for (auto& batch : mBatches)
		{
			batch->Render();
		}
	}
};