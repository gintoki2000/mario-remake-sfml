#include "onewayplatform.h"
#include "player.h"
COneWayPlatform *COneWayPlatform::create(CProperties *properties)
{
    COneWayPlatform* self = new (std::nothrow) COneWayPlatform();
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

COneWayPlatform::~COneWayPlatform()
{
    std::cout<<"~COneWayPlatform()"<<std::endl;
}

void COneWayPlatform::update(TFloat deltaTime)
{
    //move(KVecRight);
}

TInt COneWayPlatform::checkCollisionWithPlayer(CPlayer *player)
{
    TFloat playerBottom = player->getY() + player->getHeight();
    TFloat ownBottom = getY() + getHeight();
    TBoolean isPlayerOnTop = playerBottom < ownBottom;
    TBoolean isPlayerFallingDown = player->getSpeedY() > 0.f;
    if(isPlayerOnTop && isPlayerFallingDown && (ownBottom - playerBottom) < getHeight())
    {
        TBoolean isColliedX = ((player->getX() > getX()) && (player->getX() < getX() + getWidth()))
                || ((getX() > player->getX()) && (getX() < player->getX() + player->getWidth()));
        if(isColliedX)
        {
            player->move(0.f, getY() - playerBottom);
            player->setSpeedY(0.f);
            return EFloorCollision;
        }
    }
    return ENoCollision;
}

COneWayPlatform::COneWayPlatform()
{

}

TBoolean COneWayPlatform::init(CProperties *properties)
{

    return CPlatform::init(properties);
}
