#include "enemy.h"
#include "marioworld.h"
#include "tiledmap.h"
#include "property.h"
#include "block.h"
const TFloat CEnemy::KGravityAcceleration = KBlockSize * 13.f;
CEnemy::CEnemy()
{

}

const sf::Vector2f &CEnemy::getSpeed() const
{
    return mSpeed;
}

void CEnemy::setSpeed(const sf::Vector2f &speed)
{
    mSpeed = speed;
}

void CEnemy::setSpeedX(const TFloat &speedX)
{
    mSpeed.x = speedX;
}

void CEnemy::setSpeedY(const TFloat &speedY)
{
    mSpeed.y = speedY;
}

const TFloat &CEnemy::getSpeedX() const
{
    return mSpeed.x;
}

const TFloat &CEnemy::getSpeedY() const
{
    return mSpeed.y;
}

void CEnemy::addImpluse(const sf::Vector2f &impluse)
{
    mSpeed += impluse;
}

void CEnemy::updatePhysics(TFloat deltaTime)
{
    addImpluse(KVecDown * KGravityAcceleration * deltaTime);
    move(getSpeed() * deltaTime);
}

void CEnemy::updateCollision(TFloat deltaTime)
{

    mCollisionStates = ENoCollision;
    CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
    CTiledMap* tiledlayer = world->getTiledMap();
    sf::Vector2f moveDiff;
    mCollisionStates |= tiledlayer->checkCollisison(getPosition(), getSize(), getSpeed() * deltaTime, moveDiff);
    move(moveDiff);
}

void CEnemy::killOther()
{
       CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
       sf::FloatRect ownBounds = getBounds();
       for(const auto& other : world->getEnemies()->captureChildren())
       {
           CEnemy* enemy = static_cast<CEnemy*>(other);
           if(!enemy->isDead() && enemy != this)
           {
               const sf::FloatRect& otherBounds = enemy->getBounds();
               if(ownBounds.intersects(otherBounds))
               {
                   enemy->killedByOther(this);
               }
           }
       }
}

TBoolean CEnemy::isDead() const
{
    return mIsDead;
}


TBoolean CEnemy::init(CProperties* properties)
{
    CGameObject::init(properties);
    mIsDead = false;
    mCollisionStates = ENoCollision;
    setActiveDis(EFifitTeenMeter);
    return true;
}

TBoolean CEnemy::hasNextUnderBlock()
{
    CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
    CTiledMap* tiledMap = world->getTiledMap();
    if(getSpeedX() < 0.f)
    {
        TInt bottom = static_cast<TInt>((getY() + getHeight() + 1.f) / tiledMap->getBlockHeight());
        TInt left = static_cast<TInt>((getX() - 1.f) / tiledMap->getBlockWidth());
        CBlock* block = tiledMap->getBlock(left, bottom);
        return block && block->isRigid();
    }
    else if(getSpeedX() > 0.f)
    {
        TInt bottom = static_cast<TInt>((getY() + getHeight() + 1.f) / tiledMap->getBlockHeight());
        TInt right = static_cast<TInt>((getX() + getWidth() + 1.f) / tiledMap->getBlockWidth());
        CBlock* block = tiledMap->getBlock(right, bottom);
        return block && block->isRigid();
    }

    return false;
}

void CEnemy::setIsDead(TBoolean isDead)
{
    mIsDead = isDead;
}




