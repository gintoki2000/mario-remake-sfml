#include "camera.h"

Camera::Camera():
    sf::View()
{

}

Camera::Camera(const sf::Vector2f &center, const sf::Vector2f &size):
    sf::View(center, size)
{

}

Camera::Camera(const sf::FloatRect &rect):
    sf::View(rect)
{

}

TFloat Camera::getTop() const
{
    return getCenter().y - getSize().y/2;
}

TFloat Camera::getLeft() const
{
    return getCenter().x - getSize().x/2;
}

TFloat Camera::getBottom() const
{
    return getCenter().y + getSize().y/2;
}

TFloat Camera::getRight() const
{
    return getCenter().x + getSize().x/2;
}


