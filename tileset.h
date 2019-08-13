#ifndef TILESET_H
#define TILESET_H
#include <map>
#include <cstdint>
#include "tile.h"
#include "base.h"
class CTileset final : CObject
{
public:
    CTileset(const CTileset&) = delete;
    CTileset operator=(CTileset&) = delete;

    CTileset();

    ~CTileset();

    void putTile(TUInt id, CTile* tile);

    CTile* getTile(TUInt id) const;

    void reomoveTile(TUInt id);

    void setName(const std::string& name);

    const std::string& getName() const;

    void setResource(sf::Texture* resource);

    void setTextureRegions(const std::vector<TextureRegion *> &textureRegions);

    const std::map<TUInt, CTile*>& getItems() const;

private:
    std::map<TUInt, CTile*> mTiles;
    std::vector<TextureRegion*> mTextureRegions;
    std::string mName;
    sf::Texture* mResource;
};

#endif // TILESET_H
