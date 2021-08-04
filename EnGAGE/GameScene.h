#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"

class GameScene : public Scene {
	TileMap mMap;
	Player mPlayer;

	bool mOpenPauseMenu = false;
public:
	GameScene(const std::string& save) noexcept;

	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};