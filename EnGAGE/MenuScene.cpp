#include "pch.h"
#include "MenuScene.h"

#include "Globals.h"
#include "EditScene.h"
#include "GameScene.h"

#include "imgui/imgui.h"

void MenuScene::Update(float delta) noexcept
{
}

void MenuScene::Render() noexcept
{
}

void MenuScene::ImGui() noexcept
{
	ImGui::Begin("Menu(not fucking finished yet)");


	if (ImGui::Button("Game")) {
		ImGui::OpenPopup("StartGame");
	}
	if (ImGui::Button("Edit")) {
		Globals::ChangeScene<EditScene>();
	}
	if (ImGui::Button("Close(Do nothing, just press the X on the upper right corner)")) {

	}

	if (ImGui::BeginPopup("StartGame")) {
		for (const auto& entry : std::filesystem::directory_iterator("Assets/Saves/")) {
			if (ImGui::Selectable(entry.path().u8string().c_str()) && entry.exists()) {
				Globals::ChangeScene<GameScene>(entry.path().u8string());
			}
		}

		ImGui::EndPopup();
	}
	ImGui::End();
}
