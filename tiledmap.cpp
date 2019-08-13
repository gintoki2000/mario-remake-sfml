#include "tiledmap.h"
#include "block.h"
#include "marioworld.h"
#include "tilesets.h"
#include <cassert>
#include <map>
#include <functional>
#include "animatedtile.h"
#include "gameobject.h"
#include <algorithm>


CTiledMap *CTiledMap::create(TInt width, TInt height, TInt blockWidth, TInt blockHeight)
{
    CTiledMap* self = new (std::nothrow) CTiledMap();
    if(self && self->init(width, height, blockWidth, blockHeight))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CTiledMap::~CTiledMap()
{
    for(TInt x = 0; x < getWidth(); x++)
    {
        for(TInt y = 0; y < getHeight(); y++)
        {
            CBlock* block = getBlock(x,y);
            if(block)
            {
                delete block;
            }
        }
    }
    delete [] mBlocks;

}

void CTiledMap::setBlock(TInt x, TInt y, CBlock *block)
{
    if(x < 0 || x >= getWidth()) return;
    if(y < 0 || y >= getHeight()) return;
    TInt blockIndex = x + y*getWidth();
    CBlock* oldBlock = mBlocks[blockIndex];
    if(oldBlock) delete  oldBlock;
    mBlocks[blockIndex] = block;

    if(mBlocks[blockIndex]) mBlocks[blockIndex]->setParent(this);
}


CBlock *CTiledMap::getBlock(TInt x, TInt y) const
{
    if(x < 0 || x >= getWidth()) return nullptr;
    if(y < 0 || y >= getHeight()) return nullptr;
    return mBlocks[x + y*getWidth()];
}

CBlock *CTiledMap::getBlock(const sf::Vector2i &blockPosition) const
{
    return getBlock(blockPosition.x, blockPosition.y);
}



TInt CTiledMap::getWidth() const
{

    return mWidth;
}

TInt CTiledMap::getHeight() const
{
    return mHeight;
}

const TFloat& CTiledMap::getBlockWidth() const
{
    return mBlockSize.x;
}

const TFloat& CTiledMap::getBlockHeight() const
{
    return mBlockSize.y;
}

const sf::Vector2f &CTiledMap::getBlockSize() const
{
    return mBlockSize;
}



void CTiledMap::destroyBlock(TInt x, TInt y)
{
    if(x < 0 || x >= getWidth()) return;
    if(y < 0 || y >= getHeight()) return;
    TInt blockIndex = x + y*getWidth();
    if(mBlocks[blockIndex]) delete mBlocks[blockIndex];
    mBlocks[blockIndex] = nullptr;
}

sf::Vector2i CTiledMap::pixelToBlockCorrds(const sf::Vector2f &pixelCoords) const
{
    return sf::Vector2i(static_cast<TInt>(pixelCoords.x / getBlockSize().x), static_cast<TInt>(pixelCoords.y / getBlockSize().y));
}

sf::Vector2f CTiledMap::blockToPixelCoords(const sf::Vector2i &blockCoords) const
{
    return sf::Vector2f(static_cast<TFloat>(blockCoords.x) * getBlockSize().x, static_cast<TFloat>(blockCoords.y) * getBlockSize().y);
}

void CTiledMap::update(TFloat deltaTime)
{
    //update animation
    CAnimatedTile::updateAnimationBaseTime();
    //update blocks
    const Camera& camera= getStage()->getCamera();
    const TInt left = static_cast<TInt>(camera.getLeft() / getBlockSize().x) - 1;
    const TInt right = static_cast<TInt>(camera.getRight() / getBlockSize().x) + 1;
    const TInt top = static_cast<TInt>(camera.getTop() / getBlockSize().y) - 1;
    const TInt bottom = static_cast<TInt>(camera.getBottom() / getBlockSize().y) + 1;
    for(TInt x = left ; x < right; x++)
    {
        for(TInt y = top; y < bottom; y++)
        {
            CBlock* block = getBlock(x, y);
            if(!block) continue;
            block->update(deltaTime);
            if(block->isDestroyed()) destroyBlock(x, y);
        }
    }
}

void CTiledMap::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    drawBlocks(renderTarget, renderStates);//draw blocks
    CNode::draw(renderTarget, renderStates);//draw children
}




sf::Vector2f CTiledMap::checkCollision(const sf::Vector2f &objectPosition, const sf::Vector2f &objectSize, const sf::Vector2f &objectSpeed, TFloat deltaTime, TInt &coliisionStates)
{
    sf::Vector2f newObjectPosition(objectPosition);
    const TFloat blockWidth = getBlockSize().x;
    const TFloat blockHeight = getBlockSize().y;
    newObjectPosition.x -= objectSpeed.x * deltaTime;
    for(TInt x = static_cast<TInt>(newObjectPosition.x / blockWidth); x < (newObjectPosition.x + objectSize.x) / blockWidth; ++x)
    {
        for (TInt y = static_cast<TInt>(newObjectPosition.y / blockHeight); y < (newObjectPosition.y + objectSize.y) / blockHeight; ++y)
        {
            CBlock* block = getBlock(x, y);
            if(block && block->isRigid())
            {
                if(objectSpeed.y >= 0.f)
                {
                    newObjectPosition.y = y * blockHeight - objectSize.y;
                    coliisionStates |= EFloorCollision;
                }
                else
                {
                    newObjectPosition.y = (y + 1) * blockHeight;
                    coliisionStates |= ECeilCollision;
                }
            }
        }
    }
    newObjectPosition.x += objectSpeed.x * deltaTime;

    for(TInt x = static_cast<TInt>(newObjectPosition.x / blockWidth); x < (newObjectPosition.x + objectSize.x) / blockWidth; ++x)
    {
        for(TInt y = static_cast<TInt>(newObjectPosition.y / blockHeight); y < (newObjectPosition.y + objectSize.y) / blockHeight; ++y)
        {
            CBlock* block = getBlock(x, y);
            if(block && block->isRigid())
            {
                if(objectSpeed.x > 0.f)
                {
                    newObjectPosition.x = x * blockWidth - objectSize.x;
                    coliisionStates |= ELeftCollision;
                }
                else if(objectSpeed.x < 0.f)
                {
                    newObjectPosition.x = (x + 1) * blockWidth;
                    coliisionStates |= ERightCollision;
                }
            }
        }
    }


    return newObjectPosition - objectPosition;
}

TInt CTiledMap::checkCollisison(const sf::Vector2f &objectPosition, const sf::Vector2f &objectSize, const sf::Vector2f &delta, sf::Vector2f &move)
{
    sf::Vector2f newObjectPosition(objectPosition);
    const TFloat blockWidth = getBlockSize().x;
    const TFloat blockHeight = getBlockSize().y;
    newObjectPosition.x -= delta.x;
    TInt collisionStates = ENoCollision;
    for(TInt x = static_cast<TInt>(newObjectPosition.x / blockWidth); x < (newObjectPosition.x + objectSize.x) / blockWidth; ++x)
    {
        for (TInt y = static_cast<TInt>(newObjectPosition.y / blockHeight); y < (newObjectPosition.y + objectSize.y) / blockHeight; ++y)
        {
            CBlock* block = getBlock(x, y);
            if(block && block->isRigid())
            {
                if(delta.y >= 0.f)
                {
                    newObjectPosition.y = y * blockHeight - objectSize.y;
                    collisionStates |= EFloorCollision;
                }
                else
                {
                    newObjectPosition.y = (y + 1) * blockHeight;
                    collisionStates |= ECeilCollision;
                }
            }
        }
    }
    newObjectPosition.x += delta.x;

    for(TInt x = static_cast<TInt>(newObjectPosition.x / blockWidth); x < (newObjectPosition.x + objectSize.x) / blockWidth; ++x)
    {
        for(TInt y = static_cast<TInt>(newObjectPosition.y / blockHeight); y < (newObjectPosition.y + objectSize.y) / blockHeight; ++y)
        {
            CBlock* block = getBlock(x, y);
            if(block && block->isRigid())
            {
                if(delta.x > 0.f)
                {
                    newObjectPosition.x = x * blockWidth - objectSize.x;
                    collisionStates |= ELeftCollision;
                }
                else if(delta.x < 0.f)
                {
                    newObjectPosition.x = (x + 1) * blockWidth;
                    collisionStates |= ERightCollision;
                }
            }
        }
    }
    move = newObjectPosition - objectPosition;
    return collisionStates;
}

TInt CTiledMap::checkHorizontalCollision(const sf::Vector2f &objectPosition, const sf::Vector2f &objectSize, const TFloat &deltaX, TFloat &moveX)
{
    TInt collisionState = ENoCollision;
    TInt top = static_cast<TInt>(objectPosition.y / getBlockHeight());
    TInt bottom = static_cast<TInt>((objectPosition.y + objectSize.y) / getBlockHeight());
    TFloat newObjectX = objectPosition.x;
    TInt x = static_cast<TInt>((deltaX < 0.f ? objectPosition.x : objectPosition.x + objectSize.y) / getBlockWidth());
    for(int y = top; y < bottom; y++)
    {
        CBlock* blockXY = getBlock(x, y);
        if(blockXY && blockXY->isRigid())
        {
            if(deltaX < 0.f)
            {

                collisionState |= ELeft;
                newObjectX =  (x+1) * getBlockWidth();
            }
            else if(deltaX > 0.f)
            {
                collisionState |= ERight;
                newObjectX = x * getBlockWidth() - objectSize.x;
            }
        }
    }
    moveX = newObjectX - objectPosition.x;
    if(moveX != 0.f)
    {

    }
    return collisionState;
}

TInt CTiledMap::checkVerticalCollision(const sf::Vector2f &objectPosition, const sf::Vector2f &objectSize, const TFloat &deltaY, TFloat &moveY)
{
    TInt collisionState = ENoCollision;
    TInt y = static_cast<TInt>((deltaY >= 0.f ? objectPosition.y + objectSize.y : objectPosition.y) / getBlockHeight());
    TInt left = static_cast<TInt>(objectPosition.x / getBlockWidth());
    TInt right = static_cast<TInt>((objectPosition.x + objectSize.x) / getBlockWidth());
    TFloat newObjectY = objectPosition.y;
    for(int x = left ; x < right; x++)
    {
        CBlock* blockXY = getBlock(x, y);
        if(blockXY && blockXY->isRigid())
        {
            if(deltaY >= 0.f)
            {
                collisionState |= EFloorCollision;
                newObjectY = y * getBlockHeight() - objectSize.y;
            }
            else
            {
                collisionState |= ECeilCollision;
                newObjectY = (y +1) * getBlockHeight();
            }
        }
    }
    moveY = newObjectY - objectPosition.y;
    return collisionState;
}


CTiledMap::CTiledMap()
{

}

void CTiledMap::drawBlocks(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
    const auto& camera = getStage()->getCamera();
    renderStates.transform *= getTransform();

    const TInt left      =   static_cast<TInt>(camera.getLeft() / getBlockSize().x) - 1;
    const TInt right     =   static_cast<TInt>(camera.getRight() / getBlockSize().x) + 1;
    const TInt top       =   static_cast<TInt>(camera.getTop() / getBlockSize().y) - 1;
    const TInt bottom    =   static_cast<TInt>(camera.getBottom() / getBlockSize().y) + 1;

    for(TInt x = left ; x < right; x++)
    {
        for(TInt y = top; y < bottom; y++)
        {
            CBlock* block = getBlock(x, y);
            if(block) renderTarget.draw(*block, renderStates);
        }
    }


}

TBoolean CTiledMap::init(TInt width, TInt height, TInt blockWidth, TInt blockHeight)
{
    mWidth = width;
    mHeight = height;
    mBlockSize = sf::Vector2f(blockWidth, blockHeight);
    mBlocks = new (std::nothrow) CBlock*[width * height];
    if(!mBlocks) return false;
    for(TInt x = 0; x < width; x++)
    {
        for(TInt y = 0; y < height; y++)
        {
            mBlocks[x + y*width] = nullptr;
        }
    }
    return true;
}


