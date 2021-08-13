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

	if (Events::IsButtonDown(0)) {
		mTowerTest.emplace_back(glm::vec2{ mPlayer.GetCursor().x, mPlayer.GetCursor().y },
			Asset::GetTexture("Assets/Textures/tower_generic_base.png"),
			Asset::GetTexture("Assets/Textures/tower_generic_cannon.png"), 0.0f);
	}
	std::for_each(mTowerTest.begin(), mTowerTest.end(), [&](auto& tower) {
		tower.Update(mWaveManager.GetEnemies());
		});
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mWaveManager.GetEnemies());
	
	mSpriteRenderer.Render(mTowerTest);
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
