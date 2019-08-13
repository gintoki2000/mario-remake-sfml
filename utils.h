
#ifndef UTILS_H
#define UTILS_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>
#include <SFML/Graphics/Shader.hpp>
#include "base.h"

#define LOG(msg)\
    do{\
    std::cout<<"[LOG]: "<<msg<<std::endl;\
    }while(0)
#define LOG_ERR(err_msg)\
    do{\
    std::cout<<"[ERROR]: "<<err_msg<<'\t'<<__FILE__<<'('<<__LINE__<<')'<<std::endl;\
    }while(0)
enum TDirection
{
    ENone = 0, ELeft = 1<<1, ERight = 1<<2, EUp = 1<<3, EDown = 1<<4
};

enum TCollisionState
{
    ENoCollision = 0,
    ELeftCollision = 1<<0,
    ERightCollision = 1<<1,
    EAboveCollision = 1<<2,
    EBelowCollision = 1<<3,
    EFloorCollision = EBelowCollision,
    ECeilCollision = EAboveCollision
};

static sf::Vector2f KVecLeft = sf::Vector2f(-1.f, 0.f);
static sf::Vector2f KVecRight = sf::Vector2f(1.f, 0.f);
static sf::Vector2f KVecUp = sf::Vector2f(0.f, -1.f);
static sf::Vector2f KVecDown = sf::Vector2f(0.f, 1.f);
static sf::Vector2f KVecZero = sf::Vector2f(0.f, 0.f);

static TFloat KBlockSize = 32.f;

void getIntersection(const sf::FloatRect& rectA, const sf::FloatRect& rectB, sf::Vector2f& delta, sf::Vector2f& TIntersection);

namespace math
{
template <typename T>
const T& clamp(const T& min, const T& max, const T& v)
{
    return std::max<T>(std::min<T>(max, v), min);
}
};

sf::Sprite flipSprite(const sf::Sprite& sprite);



#endif // UTILS_H
