#ifndef CPICKUP_H
#define CPICKUP_H
#include "gameobject.h"
class CPlayer;
class CPickUp : public CGameObject
{
public:
    CPickUp(const CPickUp&) = delete;

    CPickUp();

    virtual void onCollected(CPlayer* player) = 0;
};

#endif // CPICKUP_H
