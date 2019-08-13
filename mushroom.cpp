#include "mushroom.h"
#include "player.h"
#include "tiledmap.h"
#include "marioworld.h"
const TFloat CMushroom::KGravityAcceleration = KBlockSize * 20.f;
CMushroom *CMushroom::create(const sf::Vector2f &position)
{
    CMushroom* self = new (std::nothrow) CMushroom();
    if(self && self->init(position))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CMushroom::~CMushroom()
{
    std::cout<<"~CMushroom()"<<std::endl;
}

void CMushroom::update(TFloat deltaTime)
{
    if(mWaitingBeforeAppearing.tick())
    {
        if(mWaitingBeforeAppearing.isComplete())
        {
            mAppearingTimer.start();
            //SoundManager::play(TSoundID::EPowerUpAppear);
        }
        return;
    }
    if(mAppearingTimer.tick())
    {
        move(KVecUp * 0.5f);
        if(mAppearingTimer.isComplete())
        {
            mWaitingTimer.start();
        }
        return;
    }
    if(mWaitingTimer.tick() && mWaitingTimer.isComplete())
    {
        mSpeed = KVecRight * KBlockSize * 3.f;
    }
    mSpeed += KVecDown * KGravityAcceleration * deltaTime;
    move(mSpeed * deltaTime);
    CTiledMap* tiledlayer = static_cast<CMarioWorld*>(getStage())->getTiledMap();
    mCollisionStates = TCollisionState::ENoCollision;
    move(tiledlayer->checkCollision(localToNodeCoords(tiledlayer, KVecZero), getSize(), mSpeed, deltaTime, mCollisionStates));
    if(mCollisionStates & TCollisionState::EFloorCollision)
    {
        mSpeed.y = 0.f;
    }
    if(mCollisionStates & (TCollisionState::ELeftCollision | TCollisionState::ERightCollision))
    {
        mSpeed.x = -mSpeed.x;
    }
}

void CMushroom::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    if(mWaitingBeforeAppearing.isActive()) return;
    renderStates.transform *= getTransform();
    if(mAppearingTimer.isActive())
    {
        sf::IntRect rect = mSprite.getTextureRect();
        sf::Sprite sprite = mSprite;
        rect.height = static_cast<TInt>(mStartY - getPosition().y);
        sprite.setTextureRect(rect);
        renderTarget.draw(sprite, renderStates);
        return;
    }

    renderTarget.draw(mSprite, renderStates);
}

void CMushroom::onCollected(CPlayer *player)
{
    if(!mAppearingTimer.isActive() && !mWaitingTimer.isActive())
    {
        if(player->isSmall())
        {
            player->promote();

        }
        else
        {
            SoundManager::play(TSoundID::EOneUp);
        }
        remove();
    }
}

CMushroom::CMushroom()
{


}

TBoolean CMushroom::init(const sf::Vector2f &position)
{
    setPosition(position);
    setSize(32.f, 32.f);
    mAppearingTimer = TTimer(64);
    mWaitingTimer = TTimer(6);
    mWaitingBeforeAppearing = TTimer(15);
    mWaitingBeforeAppearing.start();
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mSprite.setTexture(*texture);
    mSprite.setTextureRect(sf::IntRect(128, 150, 32, 32));
    mStartY = getPosition().y;
    SoundManager::play(TSoundID::EPowerUpAppear);
    return true;
}


