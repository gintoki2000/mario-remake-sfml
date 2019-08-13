#include "piranhaplant.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "player.h"
#include "marioworld.h"
#include <cmath>
CPiranhaPlant *CPiranhaPlant::create(CProperties* properties)
{
    CPiranhaPlant* self = new (std::nothrow) CPiranhaPlant();
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CPiranhaPlant::~CPiranhaPlant()
{
    std::cout<<"~CPiranhaPlant()"<<std::endl;
}

void CPiranhaPlant::update(TFloat deltaTime)
{
    mAnimationTicks++;

    if(mMoving.tick())
    {
        if(mDirection == TDirection::EUp)
        {
            move(KVecUp * 0.5f);
        }
        else if(mDirection == TDirection::EDown)
        {
            move(KVecDown * 0.5f);
        }
        if(mMoving.isComplete())
        {
            if(mDirection == TDirection::EUp)
            {
                mWaitingOnTop.start();
            }
            else if(mDirection == TDirection::EDown)
            {
                mIsHiddenInPipe = true;
                mWaitingOnBottom.start();
            }
        }
        return;
    }


    if(mWaitingOnTop.tick())
    {
        if(mWaitingOnTop.isComplete())
        {
            mMoving.start();
            mDirection = TDirection::EDown;
        }
        return;
    }

    if(mIsHiddenInPipe && isPlayerNearBy())
    {

        return;
    }

    if(mWaitingOnBottom.tick())
    {
        if(mWaitingOnBottom.isComplete())
        {
            mIsHiddenInPipe = false;
            mDirection = TDirection::EUp;
            mMoving.start();
        }
        return;
    }
}

void CPiranhaPlant::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    sf::Sprite sprite = mAnim.getCurrentKeyFrame(mAnimationTicks);
    renderStates.transform.combine(getTransform());

    sf::IntRect rect = sprite.getTextureRect();
    rect.height = static_cast<TInt>(mBootom - getPosition().y);
    sprite.setTextureRect(rect);
    renderTarget.draw(sprite, renderStates);
}

void CPiranhaPlant::kickedByPlayer(CPlayer *player)
{
    if(!mIsHiddenInPipe) player->getDamage();
}

void CPiranhaPlant::kickedFromUnderground()
{

}

void CPiranhaPlant::killedByFireBall(CPlayerFireBall *fireBall)
{
    if(!mIsHiddenInPipe) remove();
}

void CPiranhaPlant::killedByOther(CEnemy *other)
{

}

void CPiranhaPlant::touchPlayer(CPlayer *player)
{
    if(!mIsHiddenInPipe) player->getDamage();
}

CPiranhaPlant::CPiranhaPlant()
{

}

TBoolean CPiranhaPlant::init(CProperties* properties)
{
    CEnemy::init(properties);
    sf::Texture* texture = TextureManager::get(TTextureID::EEnemiesTextureID);
    if(!texture) return false;
    mAnim.create(texture, 0, 80, 32, 48, 2, 1);
    mAnim.setFrameDuration(10);
    mAnim.setPlayMode(TAnimation::TPlayMode::ELoop);
    mIsHiddenInPipe = true;
    mIsWaiOnBottom = true;
    mAnimationTicks = 0;

    move(0.f, 49.f);
    mBootom = getPosition().y;
    mMoving = TTimer(48 * 2);

    mDirection = TDirection::EUp;

    mWaitingOnTop = TTimer(30);
    mWaitingOnBottom = TTimer(2 * 60);
    mWaitingOnBottom.start();
    return true;
}

TBoolean CPiranhaPlant::isPlayerNearBy()
{
    const sf::Vector2f& playerPos = static_cast<CMarioWorld*>(getStage())->getPlayer()->getPosition();
    const sf::Vector2f& ownPos = getPosition();

    sf::Vector2f delta = playerPos - ownPos;

    return std::sqrt(delta.x * delta.x + delta.y * delta.y) < KBlockSize * 4.f;
}
