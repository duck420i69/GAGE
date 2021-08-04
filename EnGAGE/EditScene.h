#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileMap.h"


class EditScene : public Scene {
	TileMap mMap;
	Player mPlayer;

	std::weak_ptr<Tile<false>> mCurrentBrush;
	std::weak_ptr<Tile<true>> mCurrentLogicBrush;
public:
	EditScene() noexcept;
	~EditScene() noexcept;
	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};