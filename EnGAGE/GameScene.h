#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"
#include "SpriteRenderer.h"
#include "WaveManager.h"

class GameScene : public Scene {
	
	TileMap mMap;
	SpriteRenderer mSpriteRenderer;
	WaveManager mWaveManager;
	Player mPlayer;
	

	bool mOpenPauseMenu = false;

	
public:
	GameScene(const std::string& save) noexcept;

	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};