#pragma once

#include "Texture.h"
#include "Shader.h"
#include "SpriteSheet.h"

#include <map>
#include <string>
#include <memory>

class Asset
{
	static std::map<std::string, std::shared_ptr<Texture>> sTextures;
	static std::map<std::string, std::shared_ptr<Shader>> sShaders;
	static std::map<std::string, std::shared_ptr<SpriteSheet>> sSpriteSheets;
public:
	static std::shared_ptr<Shader> GetShader(const std::string& path) noexcept;
	static std::shared_ptr<Texture> GetTexture(const std::string& path) noexcept;

	static void AddSpriteSheets(const std::string& name, const std::shared_ptr<SpriteSheet>& sheet) noexcept;
	static std::shared_ptr<SpriteSheet> GetSpriteSheets(const std::string& name) noexcept;
	static void Destroy() noexcept;
};