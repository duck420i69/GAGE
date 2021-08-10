#include "pch.h"
#include "SaveManager.h"

#include "Logger.h"
#include "TileType.h"

void SaveManager::Save(const std::string& file_name, const TileMap& map, const std::vector<Wave>& waves)
{
	try {
		std::ofstream file;
		file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		file.open(file_name);
		unsigned int width = map.GetWidth();
		unsigned int height = map.GetHeight();
		const auto& tiles = map.GetTiles();
		const auto& logic_tiles = map.GetLogicTiles();
		file << width << "\n";
		file << height << "\n";
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				size_t pos = x + y * (size_t)width;
				auto current_tile = tiles[pos].lock();
				file << current_tile->GetID() << " ";
			}
			file << "\n";
		}
		file << "\n";
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				size_t pos = x + y * (size_t)width;
				auto current_tile = logic_tiles[pos].lock();
				file << current_tile->GetID() << " ";
			}
			file << "\n";
		}
		file << "\n";
		
		file << waves.size() << "\n";
		for (const auto& wave : waves) {
			file << wave.GetEnemies().size() << "\n";
			for (const auto& e : wave.GetEnemies()) {
				file << (unsigned int)e << " ";
			}
			file << "\n";
		}

		file.close();
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to write save file: {}, ios error: ", file_name, e.what());
	}
}

void SaveManager::Load(const std::string& file_name, TileMap& map, std::vector<Wave>& waves)
{
	try {
		std::ifstream file;
		file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		file.open(file_name);
		unsigned int width, height;
		std::vector<std::weak_ptr<Tile>> tiles;
		std::vector<std::weak_ptr<LogicTile>> logic_tiles;
		auto tile_array = TileType::GetArray();
		auto logic_tile_array = TileType::GetLogicArray();
		file >> width;
		file >> height;
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				tiles.push_back(tile_array[value]);
			}
		}

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				logic_tiles.push_back(logic_tile_array[value]);
			}
		}
		waves.clear();

		unsigned int num_waves;
		file >> num_waves;
		waves.reserve(num_waves);
		for (unsigned int i = 0; i < num_waves; i++) {
			waves.push_back(Wave());
			unsigned int enemy_count;
			file >> enemy_count;
			for (unsigned int i = 0; i < enemy_count; i++) {
				unsigned int enemy;
				file >> enemy;
				waves.back().AddEnemy((EnemyType)enemy);
			}
		}

		map.Load(width, height, tiles, logic_tiles);
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to load save file: {}, ios error: {}", file_name, e.what());
	}
}
