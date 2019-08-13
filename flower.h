#ifndef FLOWER_H
#define FLOWER_H
#include "pickup.h"
#include "texturemanager.h"
#include "soundmanager.h"
#include "player.h"
#include "timer.h"
#include "animation.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class CFlower : public CPickUp
{
public:
    static CFlower* create(const sf::Vector2f& position);

    ~CFlower();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates) override;

    void onCollected(CPlayer* player) override;
private:
    CFlower();

    TBoolean init(const sf::Vector2f& position);

    TFloat mStartY;
    TInt mAnimationTicks;

    TAnimation mAnimation;
    TTimer mAppearingTimer;
    TTimer mWaitingBeforeAppearing;
};

#endif // FLOWER_H
