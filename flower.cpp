#include "flower.h"

CFlower *CFlower::create(const sf::Vector2f &position)
{
    CFlower* self = new (std::nothrow) CFlower();
    if(self && self->init(position))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CFlower::~CFlower()
{
    std::cout<<"~CFlower()"<<std::endl;
}

void CFlower::update(TFloat deltaTime)
{
    if(mWaitingBeforeAppearing.tick())
    {
        if(mWaitingBeforeAppearing.isComplete())
        {
            mAppearingTimer.start();
        }
        return;
    }
    mAnimationTicks++;
    if(mAppearingTimer.tick())
    {
        move(KVecUp * 0.5f);
    }
}

void CFlower::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    if(mWaitingBeforeAppearing.isActive()) return;
    renderStates.transform*=getTransform();

    if(mAppearingTimer.isActive())
    {
        sf::Sprite sprite = mAnimation.getCurrentKeyFrame(mAnimationTicks);
        sf::IntRect rect = sprite.getTextureRect();
        rect.height = static_cast<TInt>(mStartY - getPosition().y);
        sprite.setTextureRect(rect);
        renderTarget.draw(sprite, renderStates);
        return;
    }
    renderTarget.draw(mAnimation.getCurrentKeyFrame(mAnimationTicks), renderStates);

}

void CFlower::onCollected(CPlayer *player)
{
    if(mAppearingTimer.isComplete())
    {
        player->promote();
        remove();
    }
}

CFlower::CFlower()
{

}

TBoolean CFlower::init(const sf::Vector2f &position)
{
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mAnimationTicks = 0;
    mAnimation.create(texture, 32, 212, 32, 32, 3, 1);
    mAnimation.setFrameDuration(64 / 6);
    mAnimation.setPlayMode(TAnimation::TPlayMode::ELoopPingpong);
    setPosition(position);
    mStartY = getPosition().y;
    mAppearingTimer = TTimer(64);
    mWaitingBeforeAppearing = TTimer(15);
    mWaitingBeforeAppearing.start();
    setSize(32.f, 32.f);
    SoundManager::play(TSoundID::EPowerUpAppear);
    return true;
}
