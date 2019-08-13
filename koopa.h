#ifndef KOOPA_H
#define KOOPA_H
#include "enemy.h"
#include "timer.h"
#include "animation.h"
#include "marioworld.h"
class CKoopa : public CEnemy
{
public:
    static CKoopa* create(CProperties* properties);

    ~CKoopa();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;

    void kickedByPlayer(CPlayer* player) override;

    void kickedFromUnderground() override;

    void killedByFireBall(CPlayerFireBall* fireBall) override;

    void killedByOther(CEnemy* other) override;

    void touchPlayer(CPlayer* player) override;


private:
    CKoopa();

    TBoolean init(CProperties* properties) override;

    inline void updateDirection();

    static const sf::Vector2f KNormalSize, KShellSize;

    enum class TState{ENone ,ENormal, EHidingInShell, EDie, ELevitating,  ECrazyShell, EClimbing, EJumping} mState;

    void setState(TState state);

    TAnimation mWalkingAni;
    TAnimation mFlyingAni;
    TAnimation mCrazyShellAni;
    TAnimation mClimbAni;

    sf::Sprite mShell;
    sf::Sprite mFalling;
    sf::Sprite mFallingShell;

    sf::Sprite* mDylingSprite;

    TBoolean mIsKilledByFireBall;
    TBoolean mIsGounded;

    TDirection mDirection;

    TInt mAnimationTicks;

    TInt mStateTicks;

    sf::Vector2f mInitPos;

    const static TInt KHiddenTime;
    const static TInt KDieTime;
    const static TInt KClimbTime;
    const static TFloat KJumpVel;
};

#endif // KOOPA_H
