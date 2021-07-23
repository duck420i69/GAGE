#include "pch.h"
#include "Asset.h"

std::map<std::string, std::shared_ptr<Texture>> Asset::sTextures;
std::map<std::string, std::shared_ptr<Shader>> Asset::sShaders;

std::shared_ptr<Shader> Asset::GetShader(const std::string& path)
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

std::shared_ptr<Texture> Asset::GetTexture(const std::string& path)
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
