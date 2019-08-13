#ifndef FIREPRATICLE_H
#define FIREPRATICLE_H
#include "particle.h"
#include "animation.h"
#include <SFML/Graphics/RenderTarget.hpp>
class CFireSplash : public CParticle
{
public:
    static CFireSplash* create(const sf::Vector2f& position);
    ~CFireSplash();
private:
    CFireSplash();
    TBoolean init(const sf::Vector2f& position);
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;
    TAnimation mAnimation;
};

#endif // FIREPRATICLE_H
