#include "pch.h"
#include "GameScene.h"

#include "Events.h"
#include "Globals.h"
#include "MenuScene.h"

GameScene::GameScene(const std::string& save) noexcept
{
	mMap.Load(save);
	mEnemyManager.Load(mMap.GetLogicTiles(), mMap.GetWidth(), mMap.GetHeight());
}

void GameScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);
	mEnemyManager.Update(delta);

	if (Events::IsKeyDownOnce(Events::KEY_ESCAPE)) {
		Globals::ChangeScene<MenuScene>();
	}
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mEnemyManager.GetEnemies());
}

void GameScene::ImGui() noexcept
{
}
