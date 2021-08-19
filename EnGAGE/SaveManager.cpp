#include "pch.h"
#include "SaveManager.h"

#include "Logger.h"
#include "TileType.h"

void SaveManager::Save(const std::string& file_name, const TileMap& map, const WaveManager& wave_manager)
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
				file << (unsigned int)tiles[pos]->type << " ";
			}
			file << "\n";
		}
		file << "\n";
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				size_t pos = x + y * (size_t)width;
				file << (unsigned int)logic_tiles[pos]->type << " ";
			}
			file << "\n";
		}
		file << "\n";
		
		file << wave_manager.GetWaves().size() << "\n";
		for (const auto& wave : wave_manager.GetWaves()) {
			file << wave.GetEnemySpeed() << "\n";
			file << wave.GetEnemySpawnDelay() << "\n";
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

void SaveManager::Load(const std::string& file_name, TileMap& map, WaveManager& wave_manager)
{
	try {
		std::ifstream file;
		file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		file.open(file_name);
		unsigned int width, height;
		std::vector<std::unique_ptr<Tile>> tiles;
		std::vector<std::unique_ptr<Tile>> logic_tiles;
		file >> width;
		file >> height;
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				tiles.emplace_back(std::move(TileManager::Get((TileType)value)));
			}
		}

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				logic_tiles.emplace_back(std::move(TileManager::Get((LogicTileType)value)));
			}
		}
		
		std::vector<Wave> waves;
		unsigned int num_waves;
		file >> num_waves;
		waves.reserve(num_waves);
		for (unsigned int i = 0; i < num_waves; i++) {
			waves.push_back(Wave());
			float enemy_speed;
			float enemy_spawn_delay;
			unsigned int enemy_count;
			file >> enemy_speed;
			file >> enemy_spawn_delay;
			file >> enemy_count;
			for (unsigned int i = 0; i < enemy_count; i++) {
				unsigned int enemy;
				file >> enemy;
				waves.back().AddEnemy((EnemyType)enemy);
			}
			waves.back().SetEnemySpawnDelay(enemy_spawn_delay);
			waves.back().SetEnemySpeed(enemy_speed);
		}

		map.Load(width, height, tiles, logic_tiles);
		wave_manager.SetWaves(waves);
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to load save file: {}, ios error: {}", file_name, e.what());
	}
}
