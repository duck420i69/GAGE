#include "pch.h"
#include "GameScene.h"

#include "Events.h"
#include "Globals.h"
#include "MenuScene.h"
#include "SaveManager.h"

GameScene::GameScene(const std::string& save) noexcept
{
	SaveManager::Load(save, mMap, mWaves);
}

void GameScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);

	if (Events::IsKeyDownOnce(Events::KEY_ESCAPE)) {
		Globals::ChangeScene<MenuScene>();
	}


	
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.EndRender();
}

void GameScene::ImGui() noexcept
{
}
