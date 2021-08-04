#include "pch.h"
#include "GameScene.h"

#include "Events.h"

GameScene::GameScene(const std::string& save) noexcept :
	mMap(),
	mPlayer()
{
	mMap.Load(save);
}

void GameScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mMap.Update(delta, mPlayer);
}

void GameScene::Render() noexcept
{
	mMap.Render();
}

void GameScene::ImGui() noexcept
{
}
