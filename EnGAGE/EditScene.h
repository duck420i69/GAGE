#pragma once

#include "Scene.h"
#include "Camera.h"
#include "TileMap.h"
#include "SpriteRenderer.h"
#include "Enemy.h"
#include "Wave.h"

class EditScene : public Scene {

	enum class Mode {
		DRAW, LOGIC
	};

	SpriteRenderer mSpriteRenderer;
	TileMap mMap;
	Camera mPlayer;

	std::vector<Wave> mWaves;
	std::weak_ptr<Tile> mCurrentBrush;
	std::weak_ptr<LogicTile> mCurrentLogicBrush;
	Mode				mCurrentMode;
public:
	EditScene() noexcept;
	~EditScene() noexcept;
	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
private:
	void SwitchMode() noexcept;
	void RotateSprite() noexcept;
	void Draw() noexcept;
};