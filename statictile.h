#ifndef STATICTILE_H
#define STATICTILE_H
#include "tile.h"

class CStaticTile : public CTile
{
public:
    CStaticTile(const CStaticTile&) = delete ;

    CStaticTile(TextureRegion* textureRegion);

    const TextureRegion& getTextureRegion() const override;
private:
    TextureRegion* mTextureRegion;
};

#endif // STATICTILE_H
