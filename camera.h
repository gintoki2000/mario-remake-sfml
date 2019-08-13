#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics/View.hpp>
#include "base.h"
//stack class
class Camera : public sf::View
{
public:
    Camera();
    Camera(const sf::Vector2f& center, const sf::Vector2f& size);
    Camera(const sf::FloatRect& rect);

    TFloat getTop() const;

    TFloat getLeft() const;

    TFloat getBottom() const;

    TFloat getRight() const;
};





#endif // CAMERA_H
