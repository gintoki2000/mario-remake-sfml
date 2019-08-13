#include "staticblock.h"
#include "tile.h"
#include "property.h"
#include <iostream>
CStaticBlock *CStaticBlock::create(const sf::Vector2f &position, CTile *tile)
{
    CStaticBlock* self = new (std::nothrow) CStaticBlock();
    if(self && self->init(position, tile))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CStaticBlock::CStaticBlock()
{

}

TBoolean CStaticBlock::init(const sf::Vector2f &position, CTile *tile)
{
    if(!CBlock::init(position, tile)) return false;
    auto rigidProperty = tile->getProperties()->get<CBoolean>("rigid");
    if(!rigidProperty)
    {
        std::cout<<"there is no rigid property(CStaticBlock::init)!"<<std::endl;
        return false;
    }
    setRigid(rigidProperty->value());
    return true;
}
