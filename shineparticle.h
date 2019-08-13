#ifndef SHINEPARTICLE_H
#define SHINEPARTICLE_H
#include "particle.h"
#include "animation.h"
class CShineParticle : public CParticle
{
public:
    static CShineParticle* create(const sf::Vector2f& position);

    ~CShineParticle();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;
private:
    CShineParticle();
    TBoolean init(const sf::Vector2f& position);
    TAnimation mAnimation;
};

#endif // SHINEPARTICLE_H
