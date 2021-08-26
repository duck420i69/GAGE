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
	mTowerMap.Update(delta, mWaveManager.GetEnemies(), mProjectileManager);
	mProjectileManager.Update(delta, mMap.GetWidth(), mMap.GetHeight(), mWaveManager.GetEnemies());

	auto& io = ImGui::GetIO();
	if (Events::IsKeyDownOnce(Events::KEY_ESCAPE) || mWaveManager.IsGameOver()) {
		Globals::ChangeScene<MenuScene>();
		return;
	}
	
	if (!io.WantCaptureMouse) {
		if (Events::IsButtonDownOnce(0)) {
			mTowerMap.PlaceTower(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentTower);
		}
		else if (Events::IsButtonDownOnce(1)) {
			mTowerMap.RemoveTower(mPlayer.GetCursor().x, mPlayer.GetCursor().y);
		}
	}
}

void GameScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mWaveManager.GetEnemies());
	mSpriteRenderer.Render(mTowerMap.GetTowers());
	mSpriteRenderer.Render(mProjectileManager.GetProjectiles());

	auto tower_texture = TowerTypeTexture::Generate(mCurrentTower);
	mSpriteRenderer.RenderOpaque(mPlayer.GetCursor().x, mPlayer.GetCursor().y, tower_texture.base_texture);
	mSpriteRenderer.EndRender();
}

void GameScene::ImGui() noexcept
{
	ImGui::Begin("Game");
	ImGui::Text("Application average FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Current round: %u", mWaveManager.GetRound());
	if (ImGui::Button("Start round")) {
		mWaveManager.StartRound();
	}

	for (unsigned int i = 1; i < (unsigned int)TowerType::COUNT; i++) {
		TowerTexture tex = TowerTypeTexture::Generate((TowerType)i);
		auto base_tex = tex.base_texture.lock();
		if (ImGui::ImageButton((ImTextureID)base_tex->GetID(), ImVec2(base_tex->GetWidth(), base_tex->GetHeight()))) {
			mCurrentTower = (TowerType)i;
		}
	}
	ImGui::End();

}
