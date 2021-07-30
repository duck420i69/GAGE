#pragma once

#include "Component.h"
#include "Texture.h"
#include "Sprite.h"
#include "Transform.h"
#include "GameObject.h"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

#include "Thirdparty/imgui/imgui.h"

class SpriteRenderer : public Component
{
	glm::vec4 mColor;
	Sprite    mSprite;

	Transform mLastTransform;
	bool mDirty;
public:
	SpriteRenderer(const glm::vec4& color) noexcept :
		mColor(color), mSprite(std::weak_ptr<Texture>()), mDirty(true)
	{}

	SpriteRenderer(const Sprite& sprite) noexcept :
		mColor({ 1, 1, 1, 1 }), mSprite(sprite), mDirty(true)
	{}

	void Start() noexcept override
	{
		mLastTransform = this->mGameObject->mTransform;
	}
	void Update(double dt) noexcept override
	{
		if (mLastTransform != this->mGameObject->mTransform)
		{
			mLastTransform = this->mGameObject->mTransform;
			mDirty = true;
		}
	}
	void ImGui() noexcept override
	{
		if (ImGui::ColorPicker4("Sprite Color", &mColor.x))
		{
			mDirty = true;
		}
	}

	inline const glm::vec4& GetColor() const noexcept { return mColor; }
	inline const std::weak_ptr<Texture>& GetTexture() const noexcept { return mSprite.GetTexture(); }
	inline const std::array<glm::vec2, 4>& GetTexCoords() const noexcept { return mSprite.GetTexCoords(); }

	inline void SetSprite(const Sprite& sprite) noexcept
	{
		if (sprite != mSprite)
		{
			mSprite = sprite;
			mDirty = true;
		}

	}
	inline void SetColor(const glm::vec4& color) noexcept
	{
		if (mColor != color)
		{
			mColor = color;
			mDirty = true;
		}
	}

	inline bool IsDirty() const noexcept { return mDirty; }
	inline void CleanDirty() noexcept { mDirty = false; }
private:
	void ParseComponent(std::ofstream& s) noexcept override {
		s << typeid(*this).name() << " "
			<< mColor.x << " "
			<< mColor.y << " "
			<< mColor.z << " "
			<< std::boolalpha << mDirty << " "
			<< mSprite.GetTexture().lock()->GetName() << " ";

		for (const glm::vec2& v : mSprite.GetTexCoords())
		{
			s	<< v.x << " "
				<< v.y << " " ;
		}
	}
};