#pragma once

#include <memory>
#include <vector>

#include "Sprite.h"

class SpriteSheet
{
	std::weak_ptr<Texture> mSheet;
	std::vector<Sprite> mSprites;
public:
	SpriteSheet(
		const std::weak_ptr<Texture>& sheet,
		int sprite_width, int sprite_height,
		int num_sprites, int spacing) noexcept :
		mSheet(sheet), mSprites()
	{
		unsigned int currentX = 0;
		unsigned int currentY = sheet.lock()->GetHeight() - sprite_height;

		for (int i = 0; i < num_sprites; i++)
		{
			float topY = (currentY + sprite_height) / (float)sheet.lock()->GetHeight();
			float rightX = (currentX + sprite_width) / (float)sheet.lock()->GetWidth();
			float leftX = currentX / (float)sheet.lock()->GetWidth();
			float bottomY = currentY / (float)sheet.lock()->GetHeight();

			mSprites.emplace_back(Sprite{ sheet, 
			   {glm::vec2{ rightX, topY },
				glm::vec2{ rightX, bottomY },
				glm::vec2{ leftX, bottomY },
				glm::vec2{ leftX, topY }}
			});
			currentX += sprite_width + spacing;
			if (currentX >= sheet.lock()->GetWidth())
			{
				currentX = 0;
				currentY += sprite_height + spacing;
			}
		}
	}

	const Sprite& operator[](int index) const noexcept
	{
		return mSprites[index];
	}
};