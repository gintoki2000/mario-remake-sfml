#ifndef MUSHROOM_H
#define MUSHROOM_H
#include "pickup.h"
#include "texturemanager.h"
#include "soundmanager.h"
#include "timer.h"
#include <SFML/Graphics/Sprite.hpp>
class CMushroom : public CPickUp
{
public:
    static CMushroom* create(const sf::Vector2f& position);

    ~CMushroom();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;

    void onCollected(CPlayer* player) override;
private:
    CMushroom();
    TBoolean init(const sf::Vector2f& position);
    sf::Sprite mSprite;
    TTimer mAppearingTimer;
    TTimer mWaitingTimer;
    TTimer mWaitingBeforeAppearing;
    TInt mCollisionStates;
    sf::Vector2f mSpeed;
    TFloat mStartY;
    static const TFloat KGravityAcceleration;
};

#endif // MUSHROOM_H
