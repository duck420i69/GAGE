#pragma once

#include "Texture.h"
#include "Shader.h"

#include <map>
#include <string>
#include <memory>

class Asset
{
	static std::map<std::string, std::shared_ptr<Texture>> sTextures;
	static std::map<std::string, std::shared_ptr<Shader>> sShaders;
public:
	static std::shared_ptr<Shader> GetShader(const std::string& path);
	static std::shared_ptr<Texture> GetTexture(const std::string& path);
};