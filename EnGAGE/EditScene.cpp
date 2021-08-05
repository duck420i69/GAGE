#include "pch.h"
#include "EditScene.h"

#include "Events.h"
#include "TileType.h"
#include "Logger.h"
#include "MenuScene.h"
#include "Globals.h"

#include <imgui/imgui.h>

#include <Windows.h>

EditScene::EditScene() noexcept :
	mCurrentBrush(TileType::GetArray()[1]), mCurrentLogicBrush(TileType::GetLogicArray()[1]), mCurrentMode(Mode::DRAW)
{
}

EditScene::~EditScene() noexcept
{
}

void EditScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mSpriteRenderer.Update(mPlayer);

	//Đổi giữa chế độ logic và draw
	if (Events::IsKeyDownOnce(Events::KEY_TAB)) {
		if (mCurrentMode == Mode::DRAW) {
			mCurrentMode = Mode::LOGIC;
		}
		else if (mCurrentMode == Mode::LOGIC) {
			mCurrentMode = Mode::DRAW;
		}
	}

	//Xoay sprite
	if (mCurrentMode == Mode::LOGIC && Events::IsKeyDownOnce(Events::KEY_R)) {
		auto logic_tile = mMap.GetLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y);
		if (TileType::NotLogicNone(logic_tile)) {
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
	}

	ImGuiIO& io = ImGui::GetIO();
	if (mMap.GetWidth() != 0 && mMap.GetHeight() != 0 && !io.WantCaptureMouse) {
		if (mCurrentBrush.lock() &&  mCurrentMode == Mode::DRAW) {
			if (Events::IsButtonDown(0)) {
				mMap.PlaceTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentBrush);
			}
			else if (Events::IsButtonDown(1)) {
				mMap.PlaceTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, TileType::NONE);
			}
		}
		if (mCurrentLogicBrush.lock() && mCurrentMode == Mode::LOGIC) {
			if (Events::IsButtonDown(0)) {
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentLogicBrush);
			}
			else if (Events::IsButtonDown(1)) {
				mMap.PlaceLogicTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, TileType::LOGIC_NONE);
			}
		}
	}

	
}

void EditScene::Render() noexcept
{
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetTiles());
	mSpriteRenderer.Render(mMap.GetWidth(), mMap.GetHeight(), mMap.GetLogicTiles());
}

void EditScene::ImGui() noexcept
{
	//ImGui::Begin("CurrentMode", 0, ImGuiWindowFlags_NoMove | ImGuiWindow);
	//ImGui::End();

	bool open_new_popup = false;
	bool open_save_popup = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(u8"File")) {

			if (ImGui::MenuItem(u8"New")) {
				open_new_popup = true;
			}
			if (ImGui::MenuItem(u8"Save")) {
				open_save_popup = true;
			}
			if (ImGui::BeginMenu(u8"Load")) {
				for (const auto& entry : std::filesystem::directory_iterator("Assets/Saves/")) {
					if (ImGui::Selectable(entry.path().u8string().c_str()) && entry.exists()) {
						mMap.Load(entry.path().u8string());
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(u8"Xóa")) {
				for (const auto& entry : std::filesystem::directory_iterator("Assets/Saves/")) {
					if (ImGui::Selectable(entry.path().u8string().c_str()) && entry.exists()) {
						std::filesystem::remove(entry);
					}
				}
				ImGui::EndMenu();
			}


			if (ImGui::Button(u8"Tắt editor")) {
				Globals::ChangeScene<MenuScene>();
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				return;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"Edit")) {
			if (ImGui::BeginMenu(u8"Brush")) {
				//Selecting brush	
				const auto tile_array = TileType::GetArray();
				ImGui::Text(u8"chọn cọ vẽ");
				auto current_texture = mCurrentBrush.lock()->GetTexture().lock();
				ImGui::Image((ImTextureID)current_texture->GetID(),
					ImVec2((float)current_texture->GetWidth(), (float)current_texture->GetHeight()));
				for (unsigned int i = 1; i < TileType::NUM_TYPES; i++) {
					auto texture = tile_array[i].lock()->GetTexture().lock();
					unsigned int texture_id = texture->GetID();
					unsigned int width = texture->GetWidth();
					unsigned int height = texture->GetHeight();
					if (ImGui::ImageButton((ImTextureID)texture_id, ImVec2((float)width, (float)height))) {
						mCurrentBrush = tile_array[i];
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("%s", tile_array[i].lock()->GetName().c_str());
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
				}


				//Selecting logic brush	
				const auto logic_tile_array = TileType::GetLogicArray();
				ImGui::Text(u8"chọn cọ vẽ logic");
				auto current_logic_texture = mCurrentLogicBrush.lock()->GetTexture().lock();
				ImGui::Image((ImTextureID)current_logic_texture->GetID(),
					ImVec2((float)current_logic_texture->GetWidth(), (float)current_logic_texture->GetHeight()));
				for (unsigned int i = 1; i < TileType::NUM_LOGIC_TYPES; i++) {
					auto texture = logic_tile_array[i].lock()->GetTexture().lock();
					unsigned int texture_id = texture->GetID();
					unsigned int width = texture->GetWidth();
					unsigned int height = texture->GetHeight();
					if (ImGui::ImageButton((ImTextureID)texture_id, ImVec2((float)width, (float)height))) {
						mCurrentLogicBrush = logic_tile_array[i];
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("%s", logic_tile_array[i].lock()->GetName().c_str());
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

		ImGui::EndMainMenuBar();
	}
	if (open_new_popup) 
		ImGui::OpenPopup(u8"NewMap");
	if (ImGui::BeginPopup(u8"NewMap")) {
		//Create new empty map
		static int width = 10, height = 10;
		ImGui::Text(u8"Tạo map");
		ImGui::InputInt(u8"Chiều rộng", &width);
		ImGui::InputInt(u8"Chiều cao", &height);
		if (ImGui::Button(u8"Tạo")) {
			mMap.LoadNew(width, height, TileType::NONE, TileType::LOGIC_NONE);
		}
		ImGui::EndPopup();
	}

	if (open_save_popup)
		ImGui::OpenPopup(u8"SaveMap");
	if (ImGui::BeginPopup(u8"SaveMap")) {
		//Save current map to file
		static constexpr unsigned int MAX_MAP_NAME_LENGTH = 256;
		static char map_name[MAX_MAP_NAME_LENGTH];
		ImGui::Text(u8"Sao lưu máp");
		ImGui::InputText(u8"Tên máp", map_name, sizeof(char) * MAX_MAP_NAME_LENGTH);
		if (ImGui::Button(u8"Sao lưu")) {
			if (strlen(map_name) != 0)
				mMap.Write("Assets/Saves/" + std::string(map_name));
		}
		ImGui::EndPopup();
	}

}

