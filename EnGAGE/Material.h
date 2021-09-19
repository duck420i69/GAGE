#pragma once

#include <assimp/scene.h>
#include <filesystem>
#include "Technique.h"

class Material {
	std::string mName;
	std::string mModelPath;
	std::vector<Technique> mTechniques;
public:
	Material(const aiMaterial& material, const std::filesystem::path& model_path) noexcept;

	inline const std::vector<Technique>& GetTechniques() const noexcept { return mTechniques; }
};