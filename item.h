#ifndef ITEM_H
#define ITEM_H
#include "gameobject.h"
#include "property.h"
class CPlayer;
enum TItemType
{
    EPlatform
};
class CItem : public CGameObject
{
public:
    virtual TInt checkCollisionWithPlayer(CPlayer* player) = 0;
protected:
    CItem();
    virtual TBoolean init(CProperties* properties);
};

#endif // ITEM_H
