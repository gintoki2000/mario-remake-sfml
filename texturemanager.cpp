#include "texturemanager.h"
#include <iostream>
#include <stdexcept>
TBoolean TextureManager::load(const std::string &fileName, TInt textureID)
{
    if(textures.find(textureID) != textures.end()) return false;
    sf::Texture* texture = new sf::Texture();
    if(texture && texture->loadFromFile(fileName))
    {
        textures.insert(std::make_pair(textureID, texture));
        return true;
    }
    delete texture;
    return false;
}

sf::Texture *TextureManager::get(TInt textureID)
{
    auto found = textures.find(textureID);
    if(found == textures.end()) return nullptr;
    return (*found).second;
}

void TextureManager::unLoad(TInt textureID)
{
    auto found = textures.find(textureID);
    if(found == textures.end()) return;
    delete (*found).second;
    textures.erase(found);
}

void TextureManager::unloadAll()
{
    for(const auto& pair : textures)
    {
        delete pair.second;
    }
}

std::map<TInt, sf::Texture*> TextureManager::textures = std::map<TInt, sf::Texture*>();
