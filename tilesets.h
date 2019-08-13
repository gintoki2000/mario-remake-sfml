#ifndef TILESETS_H
#define TILESETS_H
#include "tileset.h"
#include <vector>
class CTileSets final
{
public:
    CTileSets(const CTileSets&) = delete;
    CTileSets& operator=(const CTileSets&) = delete;

    CTileSets();

    ~CTileSets();

    void addTileset(CTileset* tileset);

    void removeTileset(TInt index);

    void removeTileSet(CTileset* tileset);

    CTileset* getTileset(const std::string& name) const;

    CTileset* getTileset(TInt index) const;

    CTile* getTile(TUInt id) const;

    const std::vector<CTileset*>& getItems() const;


private:
    std::vector<CTileset*> mTilesets;
};

#endif // TILESETS_H
