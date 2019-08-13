#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <vector>
#include <map>
#include "tmxlite/Property.hpp"
#include "property.h"
typedef  sf::Sprite TextureRegion;

class CTile
{
public:
    CTile(const CTile&) = delete;
    CTile& operator=(const CTile&) = delete;

    CTile();

    virtual ~CTile();

    virtual const TextureRegion& getTextureRegion() const = 0;

    void setType(const std::string& type);

    const std::string& getType() const;

    CProperties* getProperties();


private:
    std::string mType;
    CProperties* mProperties;
};

#endif // TILE_H
