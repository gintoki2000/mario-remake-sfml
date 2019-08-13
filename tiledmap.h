#ifndef MAP_H
#define MAP_H
#include "node.h"
#include <cstdint>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
class CBlock;
class CTileSets;
class CTile;
class CGameObject;
class CTiledMap : public CNode
{
public:
    CTiledMap(const CTiledMap&) = delete;

    static CTiledMap* create(TInt width, TInt height, TInt blockWidth, TInt blockHeight);

    ~CTiledMap();

    void setBlock(TInt x, TInt y, CBlock* block);



    CBlock* getBlock(TInt x, TInt y) const;

    CBlock* getBlock(const sf::Vector2i& blockPosition) const;

    TInt getWidth() const;

    TInt getHeight() const;

    const TFloat& getBlockWidth() const;

    const TFloat& getBlockHeight() const;


    const sf::Vector2f& getBlockSize() const;

    void destroyBlock(TInt x, TInt y);

    sf::Vector2i pixelToBlockCorrds(const sf::Vector2f& pixelCoords) const;

    sf::Vector2f blockToPixelCoords(const sf::Vector2i& blockCoords) const;

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) override;

    sf::Vector2f checkCollision(const sf::Vector2f& objectPosition, const sf::Vector2f& objectSize, const sf::Vector2f& objectSpeed, TFloat deltaTime, TInt& objectColiisionStates);

    TInt checkCollisison(const sf::Vector2f& objectPosition, const sf::Vector2f& objectSize, const sf::Vector2f& delta, sf::Vector2f& move);

    TInt checkHorizontalCollision(const sf::Vector2f& objectPosition, const sf::Vector2f& objectSize, const TFloat& deltaX, TFloat& moveX);

    TInt checkVerticalCollision(const sf::Vector2f& objectPosition, const sf::Vector2f& objectSize, const TFloat& deltaY, TFloat& moveY);

private:
    CTiledMap();
    void drawBlocks(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;

    TBoolean init(TInt width, TInt height, TInt blockWidth, TInt blockHeight);
    TInt mWidth;
    TInt mHeight;
    sf::Vector2f mBlockSize;
    CBlock** mBlocks;

};

#endif // MAP_H
