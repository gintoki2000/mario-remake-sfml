#include "statictile.h"
CStaticTile::CStaticTile(TextureRegion *textureRegion):
    mTextureRegion(textureRegion)
{

}

const TextureRegion &CStaticTile::getTextureRegion() const
{
    return *mTextureRegion;
}
