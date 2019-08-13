#ifndef GOOMBA_H
#define GOOMBA_H
#include "enemy.h"
#include "animation.h"
#include "timer.h"
#include "property.h"
class CGoomba : public CEnemy
{
public:

    ~CGoomba();

    static CGoomba* create(CProperties* goombaPorperties);

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;

    void kickedByPlayer(CPlayer* player) override;

    void kickedFromUnderground() override;

    void killedByOther(CEnemy* other) override;

    void killedByFireBall(CPlayerFireBall* fireBall) override;

    void touchPlayer(CPlayer* player) override;


private:
    CGoomba();
    bool init(CProperties* goombaPorperies) override;

    TAnimation mWalkingAnimation;
    TAnimation mCrampedAnimation;
    TAnimation mFallingAnimation;
    TAnimation mCurrentAnimation;
    TInt mAnimationTicks;
    TTimer mFallingTimer;
    TTimer mCrampingTimer;
};

#endif // GOOMBA_H
