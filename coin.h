#ifndef COIN_H
#define COIN_H
#include "pickup.h"
#include "animation.h"
#include "property.h"
#include "timer.h"
class CCoin : public CPickUp
{
public:
    static CCoin* create(CProperties* properties);

    static CCoin* createOnBlock(const sf::Vector2f& position);

    ~CCoin();

    void onCollected(CPlayer* player) override;

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;


private:
    CCoin();

    TBoolean init(CProperties* properties);

    TBoolean init(const sf::Vector2f& position);

    TInt mAnimTicks;

    TAnimation mAnim;

    TTimer mAutoRemoveTimer;
};

#endif // COIN_H
