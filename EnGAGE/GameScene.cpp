#include "pch.h"
#include "GameScene.h"

#include "Events.h"
#include "Globals.h"
#include "MenuScene.h"
#include "SaveManager.h"
#include "Asset.h"
#include "TowerType.h"

#include "imgui/imgui.h"

GameScene::GameScene(const std::string& save) noexcept
{
	SaveManager::Load(save, mMap, mWaveManager);
	mWaveManager.Load(mMap);
	mTowerMap.Load(mMap);
}

void GameScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);
	mWaveManager.Update(delta, mMap);
	mTowerMap.Update(delta, mWaveManager.GetEnemies());

	if (Events::IsKeyDownOnce(Events::KEY_ESCAPE) || mWaveManager.IsGameOver()) {
		Globals::ChangeScene<MenuScene>();
		return;
	}
	
	if (Events::IsButtonDownOnce(0)) {
		mTowerMap.PlaceTower(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentTower);
	}
	
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mWaveManager.GetEnemies());
	mSpriteRenderer.Render(mTowerMap.GetTowers());

	auto tower_texture = TowerTypeTexture::Generate(mCurrentTower);
	mSpriteRenderer.RenderOpaque(mPlayer.GetCursor().x, mPlayer.GetCursor().y, tower_texture.base_texture);
	mSpriteRenderer.EndRender();
}

void GameScene::ImGui() noexcept
{
	ImGui::Begin(u8"Game");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Current round: %u", mWaveManager.GetRound());
	if (ImGui::Button(u8"Start round")) {
		mWaveManager.StartRound();
	}

	for (unsigned int i = 0; i < (unsigned int)TowerType::COUNT; i++) {
		TowerTexture tex = TowerTypeTexture::Generate((TowerType)i);
		auto base_tex = tex.base_texture.lock();
		if (ImGui::ImageButton((ImTextureID)base_tex->GetID(), ImVec2(base_tex->GetWidth(), base_tex->GetHeight()))) {
			mCurrentTower = (TowerType)i;
		}
	}
	ImGui::End();
}
