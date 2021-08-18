#pragma once

#include "TileMap.h"
#include "WaveManager.h"


class SaveManager {
public:
	static void Save(const std::string& file_name, const TileMap& map, const WaveManager& wave_manager);
	static void Load(const std::string& file_name, TileMap& map, WaveManager& wave_manager);
};