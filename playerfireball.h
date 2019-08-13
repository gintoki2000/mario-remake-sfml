#ifndef MARIOFIREBALL_H
#define MARIOFIREBALL_H
#include "gameobject.h"
#include "animation.h"
#include "timer.h"
#include "tiledmap.h"
class CPlayerFireBall : public CGameObject
{
private:
    CPlayerFireBall();
    TBoolean init(TInt direction);
public:
    static CPlayerFireBall* create(TInt direction);

    ~CPlayerFireBall();
    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates)override;

    void setSpeed(const sf::Vector2f &speed);

    const sf::Vector2f& getSpeed() const;

private:
    sf::Vector2f mSpeed;
    TAnimation mAnimation;
    TTimer mTimer;
    CTiledMap* mTiledMap;
    TInt mCollisionStates;
    TInt mAnimationTicks;
    static const TFloat KGravityAcceleration;
    static const TFloat KRecoilSpeed;
};

#endif // MARIOFIREBALL_H
