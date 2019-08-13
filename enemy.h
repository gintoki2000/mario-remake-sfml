#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"
#include "property.h"
class CPlayer;
class CPlayerFireBall;
class CMarioWorld;
class CEnemy : public CGameObject
{
public:
    CEnemy(const CEnemy&) = delete;

    CEnemy();

    virtual void kickedByPlayer(CPlayer* player) = 0;

    virtual void kickedFromUnderground() = 0;

    virtual void killedByFireBall(CPlayerFireBall* fireBall) = 0;

    virtual void killedByOther(CEnemy* other) = 0;

    virtual void touchPlayer(CPlayer* player) = 0;

    const sf::Vector2f& getSpeed() const;

    void setSpeed(const sf::Vector2f& speed);

    void setSpeedX(const TFloat& speedX);

    void setSpeedY(const TFloat& speedY);

    const TFloat& getSpeedX() const;

    const TFloat& getSpeedY() const;

    void addImpluse(const sf::Vector2f& impluse);

    virtual void updatePhysics(TFloat deltaTime);

    virtual void updateCollision(TFloat deltaTime);

    virtual void killOther();

    TBoolean isDead() const;

    void setIsDead(TBoolean isDead);

protected:
    virtual TBoolean init(CProperties* properties);

    TBoolean hasNextUnderBlock();
    sf::Vector2f mSpeed;
    static const TFloat KGravityAcceleration;
    TInt mCollisionStates;
private:
    TFloat mActiveDis;
    TBoolean mIsDead;
};

#endif // ENEMY_H
