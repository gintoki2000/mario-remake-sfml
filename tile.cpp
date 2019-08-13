#include "tile.h"


CTile::CTile():
    mProperties(nullptr)
{
}

CTile::~CTile()
{

}

void CTile::setType(const std::string &type)
{
    mType = type;
}

const std::string &CTile::getType() const
{
    return mType;
}

CProperties* CTile::getProperties()
{
    if(!mProperties)
    {
        mProperties = new CProperties();
    }
    return mProperties;
}



