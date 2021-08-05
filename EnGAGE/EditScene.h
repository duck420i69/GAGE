#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"
#include "SpriteRenderer.h"

class EditScene : public Scene {

	enum class Mode {
		DRAW, LOGIC
	};

	SpriteRenderer mSpriteRenderer;
	TileMap mMap;
	Player mPlayer;

	std::weak_ptr<Tile> mCurrentBrush;
	std::weak_ptr<LogicTile> mCurrentLogicBrush;
	Mode				mCurrentMode;
public:
	EditScene() noexcept;
	~EditScene() noexcept;
	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};