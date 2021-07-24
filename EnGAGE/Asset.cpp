#include "pch.h"
#include "Asset.h"

std::map<std::string, std::shared_ptr<Texture>> Asset::sTextures;
std::map<std::string, std::shared_ptr<Shader>> Asset::sShaders;
std::map<std::string, std::shared_ptr<SpriteSheet>> Asset::sSpriteSheets;

std::shared_ptr<Shader> Asset::GetShader(const std::string& path) noexcept
{
    if (sShaders.count(path) == 0)
    {
        //Load new shader
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->LoadVertex(path + "_VS.glsl");
        shader->LoadFragment(path + "_FS.glsl");
        shader->Create();
        sShaders.emplace(path, shader);
        return shader;
    }
    else
    {
        return sShaders[path];
    }
}

std::shared_ptr<Texture> Asset::GetTexture(const std::string& path) noexcept
{
    if (sTextures.count(path) == 0)
    {
        //Load new shader
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
        sTextures.emplace(path, texture);
        return texture;
    }
    else
    {
        return sTextures[path];
    }
}

void Asset::AddSpriteSheets(const std::string& name, const std::shared_ptr<SpriteSheet>& sheet) noexcept
{
    if (sSpriteSheets.count(name) == 0)
    {
        sSpriteSheets.emplace(name, sheet);
    }
}

std::shared_ptr<SpriteSheet> Asset::GetSpriteSheets(const std::string& name) noexcept
{
    if (sSpriteSheets.count(name) == 0)
    {
        assert(!"Sprite sheet not added !");
    }
    return sSpriteSheets[name];
}

void Asset::Destroy() noexcept
{
    sSpriteSheets.clear();
    sShaders.clear();
    sTextures.clear();
}
