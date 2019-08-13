#ifndef ONEWAYPLATFORM_H
#define ONEWAYPLATFORM_H
#include "platform.h"

class COneWayPlatform : public CPlatform
{
public:
    static COneWayPlatform* create(CProperties* properties);

    ~COneWayPlatform();

    void update(TFloat deltaTime);

    TInt checkCollisionWithPlayer(CPlayer* player);
private:
    COneWayPlatform();
    TBoolean init(CProperties* properties);
};

#endif // ONEWAYPLATFORM_H
