#include "pch.h"
#include "EditScene.h"

#include "Events.h"
#include "TileType.h"
#include "Logger.h"

#include <imgui/imgui.h>

EditScene::EditScene() noexcept 
{
}

EditScene::~EditScene() noexcept
{
}

void EditScene::Update(float delta) noexcept
{
	mPlayer.Update(delta);
	mMap.Update(delta, mPlayer);

	if (mCurrentBrush.lock() && mMap.GetWidth() != 0 && mMap.GetHeight() != 0) {
		if (Events::IsButtonDown(0)) {
			mMap.PlaceTile(mPlayer.GetCursor().x, mPlayer.GetCursor().y, mCurrentBrush);
		}
	}
}

void EditScene::Render() noexcept
{
	mMap.Render();
}

void EditScene::ImGui() noexcept
{
	ImGui::Begin("Editor");

	//Create new empty map
	static int width = 10, height = 10;
	ImGui::Text("Create new map");
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Height", &height);
	if (ImGui::Button("Create")) {
		mMap.LoadNew(width, height);
	}
	ImGui::Separator();


	//Save current map to file
	static constexpr unsigned int MAX_MAP_NAME_LENGTH = 256;
	static char map_name[MAX_MAP_NAME_LENGTH];
	ImGui::Text("Save map");
	ImGui::InputText("Map name", map_name, sizeof(char) * MAX_MAP_NAME_LENGTH);
	if (ImGui::Button("Save")) {
		SaveToFile(map_name);
	}
	ImGui::Separator();

	//Selecting brush
	
	const auto tile_array = TileType::GetArray();
	ImGui::Text("Selecting brush");
	for (unsigned int i = 0; i < TileType::NUM_TYPES; i++) {
		auto texture = tile_array[i].lock()->GetTexture().lock();
		unsigned int texture_id = texture->GetID();
		unsigned int width = texture->GetWidth();
		unsigned int height = texture->GetHeight();
		if (ImGui::ImageButton((ImTextureID)texture_id, ImVec2(width, height))) {
			mCurrentBrush = tile_array[i];
		}
		ImGui::SameLine();
	}

	ImGui::End();
}

void EditScene::SaveToFile(const std::string& name) noexcept
{
	try {
		std::ofstream file;
		file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		file.open("Assets/Saves/" + name);
		unsigned int width = mMap.GetWidth();
		unsigned int height = mMap.GetHeight();
		const auto& tiles = mMap.GetTiles();
		file << width << "\n";
		file << height << "\n";
		for (unsigned int y = 0; y < mMap.GetHeight(); y++) {
			for (unsigned int x = 0; x < mMap.GetWidth(); x++) {
				auto current_tile = tiles[x + y * width].lock();
				file << current_tile->GetID() << " ";
			}
			file << "\n";
		}
		file.close();
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to write save file: {}", e.what());
	}
}
