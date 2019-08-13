#ifndef JUMPER_H
#define JUMPER_H
#include "item.h"
#include "animation.h"
#include "timer.h"
class CJumper : public CItem
{
public:
    static CJumper* create(CProperties* properties);

    void update(TFloat deltaTime);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates);

    TInt checkCollisionWithPlayer(CPlayer* player);
private:
    CJumper();

    TBoolean init(CProperties* properties);

    TAnimation mBouncingAnim;

    TTimer     mBouncingTimer;

    sf::Sprite mNormalSprite;
};

#endif // JUMPER_H
