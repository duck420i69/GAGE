#pragma once

#include "TileMap.h"
#include "Wave.h"

class SaveManager {
public:
	static void Save(const std::string& file_name, const TileMap& map, const std::vector<Wave>& waves);
	static void Load(const std::string& file_name, TileMap& map, std::vector<Wave>& waves);
};