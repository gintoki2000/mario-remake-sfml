
#include "block.h"
#include "tile.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "marioworld.h"
#include "enemy.h"
#include "property.h"
CBlock::CBlock()
{

}

TBoolean CBlock::init(const sf::Vector2f &position, CTile *tile)
{
    mPosition = position;
    mIsDestroyed = false;
    mParent = nullptr;
    setTile(tile);
    return true;
}

CBlock *CBlock::create(const sf::Vector2f &position, CTile *tile)
{
    CBlock* self = new (std::nothrow) CBlock();
    if(self && self->init( position,tile))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CBlock::~CBlock()
{

}

void CBlock::setParent(CTiledMap *map)
{
    mParent = map;
}

CTiledMap *CBlock::getParent() const
{
    return mParent;
}

void CBlock::setTile(CTile *tile)
{
    mTile = tile;
}

CTile *CBlock::getTile() const
{
    return mTile;
}

void CBlock::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
    if(!getTile()) return;
    renderStates.transform.translate(getPosition());
    renderTarget.draw(getTile()->getTextureRegion(), renderStates);
}

void CBlock::setRigid(TBoolean isRigid)
{
    mIsRigid = isRigid;
}

TBoolean CBlock::isRigid() const
{
    return mIsRigid;
}

const sf::Vector2f &CBlock::getPosition() const
{
    return mPosition;
}

sf::FloatRect CBlock::getBounds() const
{
    if(getParent())
    {
        return sf::FloatRect(getPosition(), getParent()->getBlockSize());
    }
    return sf::FloatRect(getPosition(), KVecZero);
}

TBoolean CBlock::isDestroyed() const
{
    return mIsDestroyed;
}

void CBlock::destroy()
{
    mIsDestroyed = true;
}

void CBlock::kickEnemyAbove()
{
    sf::FloatRect bounds = getBounds();
    bounds.top -= 32.f / 4.f;
    std::vector<CNode*> enemies = static_cast<CMarioWorld*>(getParent()->getStage())->getEnemies()->captureChildren();
    for(const auto& node : enemies)
    {
        CEnemy* enemy = static_cast<CEnemy*>(node);
        if(!enemy->isDead())
        {
            sf::FloatRect enemyBounds(enemy->localToNodeCoords(getParent(), KVecZero), enemy->getSize());
            if(enemyBounds.intersects(bounds))
            {
                enemy->kickedFromUnderground();
            }
        }
    }
}
