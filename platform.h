#ifndef PLATFORM_H
#define PLATFORM_H
#include "item.h"
#include "property.h"
#include "animation.h"
#include <SFML/Graphics/RenderTarget.hpp>
class CPlatform : public CItem
{
public:
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;
protected:
    CPlatform();
    TBoolean init(CProperties* properties) override;
private:
    sf::Sprite mSprite;
};

#endif // PLATFORM_H
