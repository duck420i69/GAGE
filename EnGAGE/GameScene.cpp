#include "pch.h"
#include "GameScene.h"

#include "Events.h"
#include "Globals.h"
#include "MenuScene.h"
#include "SaveManager.h"
#include "Asset.h"

#include "imgui/imgui.h"

GameScene::GameScene(const std::string& save) noexcept
{
	SaveManager::Load(save, mMap, mWaveManager.GetWaves());
	mWaveManager.Load(mMap);
}

void GameScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);
	mWaveManager.Update(delta, mMap);

	if (Events::IsKeyDownOnce(Events::KEY_ESCAPE) || mWaveManager.IsGameOver()) {
		Globals::ChangeScene<MenuScene>();
		return;
	}

	
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mWaveManager.GetEnemies());
	mSpriteRenderer.EndRender();
}

void GameScene::ImGui() noexcept
{
	ImGui::Begin(u8"Game");
	ImGui::Text("Current round: %u", mWaveManager.GetRound());
	if (ImGui::Button(u8"Start round")) {
		mWaveManager.StartRound();
	}
	ImGui::End();
}
