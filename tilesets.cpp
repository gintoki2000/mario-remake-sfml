#include "tilesets.h"

CTileSets::CTileSets()
{

}

CTileSets::~CTileSets()
{
    for(const auto& tileset : mTilesets)
    {
        delete tileset;
    }
    mTilesets.clear();
}

void CTileSets::addTileset(CTileset *tileset)
{
    mTilesets.push_back(tileset);
}

void CTileSets::removeTileset(int index)
{
    if(index < 0) throw std::runtime_error("argument \'index\' can not be a negative number! in function CTilesets::removeTileset(int index).");
    mTilesets.erase(mTilesets.begin() + index);
}

void CTileSets::removeTileSet(CTileset *tileset)
{
    auto found = std::find(mTilesets.begin(), mTilesets.end(), tileset);
    if(found == mTilesets.end()) return;
    mTilesets.erase(found);
}

CTileset *CTileSets::getTileset(const std::string &name) const
{
    auto found = std::find_if(mTilesets.begin(), mTilesets.end(),
               [&](CTileset* tileset)->TBoolean{return tileset->getName() == name;}
    );
    if(found == mTilesets.end()) return nullptr;
    return *found;
}

CTileset *CTileSets::getTileset(int index) const
{
    if(index >= mTilesets.size()) return nullptr;
    return mTilesets[index];
}

CTile *CTileSets::getTile(TUInt id) const
{
    for(const auto& tileset : mTilesets)
    {
        CTile* tile = tileset->getTile(id);
        if(tile != nullptr) return tile;
    }
    return nullptr;
}

const std::vector<CTileset *>& CTileSets::getItems() const
{
    return mTilesets;
}


