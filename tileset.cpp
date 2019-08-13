#include "tileset.h"
#include <cassert>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
CTileset::CTileset()
{
    std::shared_ptr<std::vector<TInt>> v = std::make_shared<std::vector<TInt>>();
    std::unique_ptr<TInt> i = std::make_unique<TInt>();


}



CTileset::~CTileset()
{
    for (const auto& pair : mTiles) {
        delete pair.second;
    }
    mTiles.clear();

    delete mResource;

    for (const auto& textureRegion : mTextureRegions) {
        delete textureRegion;
    }

    mTextureRegions.clear();

}

void CTileset::putTile(TUInt id, CTile *tile)
{
    assert(mTiles.find(id) == mTiles.end());
    mTiles.insert(std::make_pair(id, tile));
}

CTile *CTileset::getTile(TUInt id) const
{
    auto found = mTiles.find(id);
    if(found == mTiles.end()) return nullptr;
    return (*found).second;
}

void CTileset::reomoveTile(TUInt id)
{
    auto found = mTiles.find(id);
    if(found == mTiles.end()) return;
    delete (*found).second;
    mTiles.erase(found);
}

void CTileset::setName(const std::string &name)
{
    this->mName = name;
}

const std::string &CTileset::getName() const
{
    return this->mName;
}

void CTileset::setResource(sf::Texture *resource)
{
    this->mResource = resource;
}

void CTileset::setTextureRegions(const std::vector<TextureRegion *> &textureRegions)
{
    mTextureRegions = textureRegions;
}

const std::map<TUInt, CTile *> &CTileset::getItems() const
{
    return mTiles;
}


