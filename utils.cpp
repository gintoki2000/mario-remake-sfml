#include "utils.h"
#include <cassert>


void getIntersection(const sf::FloatRect &rectA, const sf::FloatRect &rectB, sf::Vector2f &delta, sf::Vector2f &intersection)
{
    sf::Vector2f rectAHalfSize(rectA.width / 2.f, rectA.height / 2.f);
    sf::Vector2f rectBHalfSize(rectB.width / 2.f, rectB.height / 2.f);
    delta.x = (rectB.left + rectBHalfSize.x) - (rectA.left + rectAHalfSize.x);
    delta.y = (rectB.top + rectBHalfSize.y) - (rectA.top + rectAHalfSize.y);
    intersection.x = std::abs(delta.x) - (rectAHalfSize.x + rectBHalfSize.x);
    intersection.y = std::abs(delta.y) - (rectAHalfSize.y + rectBHalfSize.y);

}

sf::Sprite flipSprite(const sf::Sprite &sprite)
{
    sf::IntRect newRect(sprite.getTextureRect().left + sprite.getTextureRect().width, sprite.getTextureRect().top, -sprite.getTextureRect().width, sprite.getTextureRect().height);
    sf::Sprite ret(sprite);
    ret.setTextureRect(newRect);
    return ret;
}

