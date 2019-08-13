#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include "base.h"
enum TTextureID
{
    EMarioTextureID,
    EEnemiesTextureID,
    EItemsTextureID
};
class TextureManager
{
private:
    TextureManager( ) { }
public:
    static TBoolean load(const std::string& fileName, TInt textureID);

    static sf::Texture* get(TInt textureID);

    static void unLoad(TInt textureID);

    static void unloadAll();
private:
   static std::map<TInt, sf::Texture*> textures;
};

#endif // TEXTUREMANAGER_H
