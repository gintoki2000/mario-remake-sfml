#ifndef BREAKBLOCKPARTICLE_H
#define BREAKBLOCKPARTICLE_H
#include "particle.h"
#include "animation.h"
class CBreakBlockParticle : public CParticle
{
public:
    static CBreakBlockParticle* create(const sf::Vector2f& position, const sf::Vector2f& diection);

    ~CBreakBlockParticle();
    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;
private:
    CBreakBlockParticle();

    bool init(const sf::Vector2f& position, const sf::Vector2f& direction);

    TAnimation mAnimation;

    sf::Vector2f mDirection;

    static const TFloat KGravityAcceleration;
};

#endif // BREAKBLOCKPARTICLE_H
