#include "core/managers/texture_manager.h"

#include "core/gpu/texture2D.h"
#include "core/managers/resource_path.h"
#include "utils/memory_utils.h"


std::unordered_map<std::string, Texture2D*> TextureManager::mapTextures;
std::vector<Texture2D*> TextureManager::vTextures;


void TextureManager::Init(const std::string &selfDir)
{
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "default.png");
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "white.png");
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "black.jpg");
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "noise.png");
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "random.jpg");
    LoadTexture(PATH_JOIN(selfDir, RESOURCE_PATH::TEXTURES), "particle.png");
}


Texture2D* TextureManager::LoadTexture(const std::string& path, const char* fileName, const char* key, bool forceLoad, bool cacheInRAM)
{
    std::string uid = key ? std::string(key) : std::string(fileName);
    Texture2D* texture = GetTexture(uid.c_str());

    if (forceLoad || texture == nullptr)
    {
        if (texture == nullptr)
        {
            texture = new Texture2D();
        }

        texture->CacheInMemory(cacheInRAM);
        bool status = texture->Load2D((path + (fileName ? (std::string(1, PATH_SEPARATOR) + fileName) : "")).c_str());

        if (!status)
        {
            delete texture;
            return (!vTextures.empty()) ? vTextures[0] : nullptr;
        }

        vTextures.push_back(texture);
        mapTextures[uid] = texture;
    }
    return texture;
}


void TextureManager::SetTexture(std::string name, Texture2D *texture)
{
    mapTextures[name] = texture;
}


Texture2D* TextureManager::GetTexture(const char* name)
{
    if (mapTextures[name])
        return mapTextures[name];
    return NULL;
}


Texture2D* TextureManager::GetTexture(unsigned int textureID)
{
    if (textureID < vTextures.size())
        return vTextures[textureID];
    return NULL;
}


std::string TextureManager::GetNameTexture(Texture2D* texture)
{
    if (!texture)
    {
        return "";
    }

    GLuint textureID = texture->GetTextureID();

    for (auto& it : mapTextures)
    {
        if (it.second->GetTextureID() == textureID)
        {
            return it.first;
        }
    }
    return "";
}
