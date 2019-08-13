#ifndef PRIRANHAPLANT_H
#define PRIRANHAPLANT_H
#include "texturemanager.h"
#include "enemy.h"
#include "animation.h"
#include "timer.h"
class CPiranhaPlant : public CEnemy
{
public:
    static CPiranhaPlant* create( CProperties* properties);

    ~CPiranhaPlant();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;

    virtual void kickedByPlayer(CPlayer* player) override;

    virtual void kickedFromUnderground() override;

    virtual void killedByFireBall(CPlayerFireBall* fireBall) override;

    virtual void killedByOther(CEnemy* other) override;

    virtual void touchPlayer(CPlayer* player) override;
private:
    CPiranhaPlant();

    TBoolean init(CProperties* properties) override;

    TBoolean isPlayerNearBy();

    TBoolean mIsHiddenInPipe;

    TBoolean mIsWaiOnBottom;

    TAnimation mAnim;

    TDirection mDirection;

    TFloat mBootom;

    TInt mAnimationTicks;

    TTimer mMoving;
    TTimer mWaitingOnTop;
    TTimer mWaitingOnBottom;
};

#endif // PRIRANHAPLANT_H
