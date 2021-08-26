#include "pch.h"
#include "EditScene.h"

#include "Events.h"
#include "TileType.h"
#include "Logger.h"
#include "MenuScene.h"
#include "Globals.h"
#include "SaveManager.h"

#include <imgui/imgui.h>

#include <Windows.h>



void EditScene::SwitchMode() noexcept
{
	//Đổi giữa chế độ logic và draw
	if (Events::IsKeyDownOnce(Events::KEY_TAB)) {
		if (mCurrentMode == Mode::DRAW) {
			mCurrentMode = Mode::LOGIC;
		}
		else if (mCurrentMode == Mode::LOGIC) {
			mCurrentMode = Mode::DRAW;
		}
	}
}

void EditScene::RotateSprite() noexcept
{
	//Xoay sprite
	/*if (mCurrentMode == Mode::LOGIC && Events::IsKeyDownOnce(Events::KEY_R)) {
		auto logic_tile = mMap.GetLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y);
		if (TileManager::NotLogicNone(logic_tile)) {
			if (TileType::IsLogicSpawn(logic_tile)) {
				static unsigned int logic_spawn_tile_count = 0;
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, TileType::GetLogicArray()[logic_spawn_tile_count % 4 + 1]);
				logic_spawn_tile_count++;
			}
			if (TileType::IsCheckpoint(logic_tile)) {
				static unsigned int logic_checkpoint_tile_count = 0;
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, TileType::GetLogicArray()[logic_checkpoint_tile_count % 4 + 5]);
				logic_checkpoint_tile_count++;
			}
		}
	}*/
}

void EditScene::Draw() noexcept
{
	ImGuiIO& io = ImGui::GetIO();
	if (mMap.GetWidth() != 0 && mMap.GetHeight() != 0 && !io.WantCaptureMouse) {
		if (mCurrentMode == Mode::DRAW) {
			if (Events::IsButtonDown(0)) {
				mMap.PlaceTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentBrush);
			}
			else if (Events::IsButtonDown(1)) {
				mMap.PlaceTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, TileType::NONE);
			}
		} else if (mCurrentMode == Mode::LOGIC) {
			if (Events::IsButtonDown(0)) {
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentLogicBrush);
			}
			else if (Events::IsButtonDown(1)) {
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, LogicTileType::NONE);
			}
		}
	}
}

void EditScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);

	SwitchMode();
	RotateSprite();
	Draw();

}

void EditScene::Render() noexcept
{
	mSpriteRenderer.Prepare();
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetLogicTiles());


	mSpriteRenderer.EndRender();
}

void EditScene::ImGui() noexcept
{
	//ImGui::Begin("CurrentMode", 0, ImGuiWindowFlags_NoMove | ImGuiWindow);
	//ImGui::End();

	bool open_new_popup = false;
	bool open_save_popup = false;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("New")) {
				open_new_popup = true;
			}
			if (ImGui::MenuItem("Save")) {
				open_save_popup = true;
			}
			if (ImGui::BeginMenu("Load")) {
				for (const auto& entry : std::filesystem::directory_iterator("Assets/Saves/")) {
					if (ImGui::Selectable(entry.path().string().c_str()) && entry.exists()) {
						WaveManager wave_manager;
						SaveManager::Load(entry.path().string(), mMap, wave_manager);
						mWaves = std::move(wave_manager.GetWaves());
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Delete")) {
				for (const auto& entry : std::filesystem::directory_iterator("Assets/Saves/")) {
					if (ImGui::Selectable(entry.path().string().c_str()) && entry.exists()) {
						std::filesystem::remove(entry);
					}
				}
				ImGui::EndMenu();
			}


			if (ImGui::Button("Close editor")) {
				Globals::ChangeScene<MenuScene>();
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				return;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::BeginMenu("Brush")) {
				//Selecting brush	
				auto current_texture = TileManager::GetTexture(mCurrentBrush).lock();
				ImGui::Image((ImTextureID)current_texture->GetID(),
					ImVec2((float)current_texture->GetWidth(), (float)current_texture->GetHeight()));
				for (unsigned int i = 1; i < (unsigned int)TileType::COUNT; i++) {
					auto texture = TileManager::GetTexture((TileType)i).lock();
					unsigned int texture_id = texture->GetID();
					unsigned int width = texture->GetWidth();
					unsigned int height = texture->GetHeight();
					if (ImGui::ImageButton((ImTextureID)texture_id, ImVec2((float)width, (float)height))) {
						mCurrentBrush = (TileType)i;
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("%s", TileManager::Get((TileType)i).name.c_str());
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
				}
				ImGui::NewLine();
				ImGui::Separator();
				//Selecting logic brush	
				auto current_logic_texture = TileManager::GetTexture(mCurrentLogicBrush).lock();
				ImGui::Image((ImTextureID)current_logic_texture->GetID(),
					ImVec2((float)current_logic_texture->GetWidth(), (float)current_logic_texture->GetHeight()));
				for (unsigned int i = 1; i < (unsigned int)LogicTileType::COUNT; i++) {
					auto texture = TileManager::GetTexture((LogicTileType)i).lock();
					unsigned int texture_id = texture->GetID();
					unsigned int width = texture->GetWidth();
					unsigned int height = texture->GetHeight();
					if (ImGui::ImageButton((ImTextureID)texture_id, ImVec2((float)width, (float)height))) {
						mCurrentLogicBrush = LogicTileType(i);
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("%s", TileManager::Get((LogicTileType)i).name.c_str());
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
				}

				ImGui::EndMenu();
			}


			ImGui::EndMenu();
		}


		//Display current mode
		static std::string mode = "";
		switch (mCurrentMode) {
		case Mode::DRAW:
			mode = "draw";
			break;
		case Mode::LOGIC:
			mode = "logic";
			break;
		}
		ImGui::Text("Current mode: %s", mode.c_str());


		//Wave
		if (ImGui::BeginMenu("Wave")) {
			std::vector<std::string> items;

			for (unsigned int i = 0; i < mWaves.size(); i++) {
				items.push_back("Wave " + std::to_string(i));
			}
			static int item_current_idx = 0;
			const std::string& combo_preview_value = items.empty() ? "" : items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)


			if (ImGui::Button("New")) {
				mWaves.emplace_back();
			}
			ImGui::SameLine();
			if (ImGui::Button("Erase") && !mWaves.empty() && !combo_preview_value.empty()) {
				mWaves.erase(mWaves.begin() + item_current_idx);
				item_current_idx = 0;
			}

			if (ImGui::BeginCombo("Wave list", combo_preview_value.c_str()))
			{
				for (unsigned int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						item_current_idx = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
					
				}
				ImGui::EndCombo();
			}
			//Wave inspector
			Wave* current_wave = mWaves.empty() ? nullptr : &mWaves[item_current_idx];
			if (current_wave) {
				ImGui::SliderFloat("Enemy speed", &current_wave->GetEnemySpeed(), 1.0f, 20.0f);
				ImGui::SliderFloat("Enemy spawn delay", &current_wave->GetEnemySpawnDelay(), 0.01f, 3.0f);
			}

			for (unsigned int i = 0; i < (unsigned int)EnemyType::COUNT; i++) {
				auto texture = EnemyTypeTexture::Get((EnemyType)i).lock();
				if (ImGui::ImageButton((ImTextureID)texture->GetID(), ImVec2(texture->GetWidth(), texture->GetHeight())) && current_wave) {
					current_wave->AddEnemy((EnemyType)i);
				}
				ImGui::SameLine();
				
			}
			if (ImGui::Button("Remove") && current_wave) {
				current_wave->GetEnemies().pop_back();
			}
			ImGui::NewLine();
			if (current_wave) {
				
				for (unsigned int i = 0; i < current_wave->GetEnemies().size(); i++) {
					const auto& enemy = current_wave->GetEnemies()[i];
					auto texture = EnemyTypeTexture::Get(enemy).lock();
					ImGui::Image((ImTextureID)texture->GetID(), ImVec2(texture->GetWidth(), texture->GetHeight()));				
					ImGui::SameLine();

					if ((i + 1) % 10 == 0) {
						ImGui::NewLine();
					}
				}
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
	if (open_new_popup)
		ImGui::OpenPopup("NewMap");
	if (ImGui::BeginPopup("NewMap")) {
		//Create new empty map
		static int width = 10, height = 10;
		ImGui::Text("New Map");
		ImGui::InputInt("width", &width);
		ImGui::InputInt("height", &height);
		if (ImGui::Button("create")) {
			mMap.LoadNew(width, height, TileType::GRASS, LogicTileType::NONE);
		}
		ImGui::EndPopup();
	}

	if (open_save_popup)
		ImGui::OpenPopup("SaveMap");
	if (ImGui::BeginPopup("SaveMap")) {
		//Save current map to file
		static constexpr unsigned int MAX_MAP_NAME_LENGTH = 256;
		static char map_name[MAX_MAP_NAME_LENGTH];
		ImGui::Text("Save");
		ImGui::InputText("map name", map_name, sizeof(char) * MAX_MAP_NAME_LENGTH);
		if (ImGui::Button("Save")) {
			if (strlen(map_name) != 0) {
				WaveManager wave_manager;
				wave_manager.SetWaves(mWaves);
				SaveManager::Save("Assets/Saves/" + std::string(map_name), mMap, wave_manager);

			}
		}
		ImGui::EndPopup();
	}
}

