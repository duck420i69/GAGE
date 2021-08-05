#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"
#include "EnemyManager.h"
#include "SpriteRenderer.h"

class GameScene : public Scene {
	TileMap mMap;
	EnemyManager mEnemyManager;
	SpriteRenderer mSpriteRenderer;
	Player mPlayer;

	bool mOpenPauseMenu = false;
public:
	GameScene(const std::string& save) noexcept;

	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};