#ifndef PLAYER_H
#define PLAYER_H
#include "gameobject.h"
#include "timer.h"
#include "animation.h"
#include "property.h"
class CPortal;
class CMarioWorld;
class CTiledMap;

class CPlayer : public CGameObject
{
public:
    enum class TPowerUpState
    {
        ENormal, ESuper, EFire
    };
private:
    enum TAnimationState
    {
        EStanding, EWalking, EJumping, ESliping, ESiting, ESwiming, EClimbing, EDyling, EFiring, EGrowing, EDemoting
    };
public:


    static CPlayer* create(CProperties* properties);

    ~CPlayer();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;


    //<>
    void getDamage();

    void enterPortal(CPortal* portal);

    void exitPortal(CPortal* portal);

    void setSpeedX(TFloat speedX);

    void setSpeedY(TFloat speedY);

    const TFloat& getSpeedX();

    const TFloat& getSpeedY();

    //<>
    TBoolean isSmall() const;

    TBoolean isDead() const;

    TBoolean isAlive() const;

    TBoolean isFireMario() const;

    const sf::Vector2f& getSpeed() const;

    void setSpeed(const sf::Vector2f &speed);

    void addImpluse(const sf::Vector2f& impluse);

    void promote();

    TInt getDirection() const;

    void standUp();

    void sitDown();

private:
    CPlayer();
    TBoolean init(CProperties* properties);
    void inputProcess(TFloat deltaTime);
    void collisionProcess(TFloat deltaTime);
    void physicalProcess(TFloat deltaTime);
    void animationProcess(TFloat deltaTime);
    void kickBlockProcess(TFloat deltaTime);

    sf::Vector2f mSpeed;


    //state timers
    TTimer mDylingTimer;
    TTimer mPromotingTimer;
    TTimer mDemotingTimer;
    TTimer mEnteringPortalTimer;
    TTimer mExitingPortalTimer;
    TTimer mSlipingTimer;


    //normal state timers
    TTimer mFiringTimer;
    TTimer mProtectionTimer;
    TTimer mJumpingTimer;
    //

    TBoolean mIsDead;
    TBoolean mIsLeverClear;

    //
    TBoolean mIsJumping;
    TBoolean mIsClimbing;
    TBoolean mIsSiting;
    TBoolean mIsGrounding;

    TAnimation mNormalAnimations[7];
    TAnimation mSuperAnimations[7];

    TAnimation mFiringAnimation;
    TAnimation mDylingAnimation;
    TAnimation mGrowingAnimation;
    TAnimation mDemotingAnimation;




    const sf::Vector2f KSuperSize;
    const sf::Vector2f KNormalSize;
    const sf::Vector2f KSitingSize;

    
    const TFloat KMaxWalkingSpeed = KBlockSize * 6.f;
    const TFloat KWalkingAcceleration = KMaxWalkingSpeed / 0.45f;
    const TFloat KGravityAcceleration = (KBlockSize * 6.f) / 0.25f;
    const TFloat KJumpingForce = KBlockSize * 16.f;
    sf::Shader* mFireShader;
    CMarioWorld* mWorld;
    CTiledMap* mTiledMap;
    CPortal* mPortal;

    TInt mInputDirection;
    TInt mDirection;
    TInt mCollisionStates;
    TInt mAnimationTicks;

    TPowerUpState mPowerUpState;
    TAnimationState mAnimationState;

};

#endif // PLAYER_H
