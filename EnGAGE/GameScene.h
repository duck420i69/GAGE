#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"
#include "SpriteRenderer.h"
#include "Wave.h"

class GameScene : public Scene {
	
	TileMap mMap;
	SpriteRenderer mSpriteRenderer;
	Player mPlayer;

	std::vector<Wave> mWaves;
	std::vector<Enemy> mEnemies;

	bool mOpenPauseMenu = false;
public:
	GameScene(const std::string& save) noexcept;

	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};