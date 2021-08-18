#pragma once

#include "Scene.h"
#include "Camera.h"
#include "TileMap.h"
#include "SpriteRenderer.h"
#include "WaveManager.h"
#include "TowerType.h"
#include "TowerMap.h"

class GameScene : public Scene {
	
	TileMap mMap;
	SpriteRenderer mSpriteRenderer;
	WaveManager mWaveManager;
	Camera mPlayer;

	bool mOpenPauseMenu = false;

	TowerMap mTowerMap;
	TowerType mCurrentTower;
	
public:
	GameScene(const std::string& save) noexcept;

	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};