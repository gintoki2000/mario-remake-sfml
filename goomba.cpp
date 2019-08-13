#include "goomba.h"
#include "texturemanager.h"
#include "mariogame.h"
#include "player.h"
#include "marioworld.h"
#include "soundmanager.h"
#include "property.h"
CGoomba::CGoomba()
{}

CGoomba::~CGoomba()
{
    std::cout<<"~CGoomba()"<<std::endl;
}

CGoomba *CGoomba::create(CProperties* prorperties)
{
    //allocate object
    CGoomba* self = new (std::nothrow) CGoomba();

    //check and init object
    if(self && self->init(prorperties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

void CGoomba::update(TFloat deltaTime)
{

    if(mCrampingTimer.tick())
    {
        if(mCrampingTimer.isComplete())
        {
            remove();
        }
        return;
    }

    if(mFallingTimer.tick())
    {
        updatePhysics(deltaTime);
        if(mFallingTimer.isComplete())
        {
            remove();
        }
        return;
    }
    mAnimationTicks++;
    updatePhysics(deltaTime);
    updateCollision(deltaTime);
    if(mCollisionStates & EFloorCollision)
    {
        mSpeed.y = 0.f;
    }

    //hasNextUnderBlock() ? setDebugColor(sf::Color::White) : setDebugColor(sf::Color::Red);

    if(mCollisionStates & (ELeftCollision | ERightCollision) || !hasNextUnderBlock())
    {
        mSpeed.x = -mSpeed.x;
    }
}

void CGoomba::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform*=getTransform();
    renderTarget.draw(mCurrentAnimation.getCurrentKeyFrame(mAnimationTicks), renderStates);
}

void CGoomba::kickedByPlayer(CPlayer *player)
{
    mCurrentAnimation = mCrampedAnimation;
    setIsDead(true);
    mCrampingTimer.start();
    setActiveDis(-1);
    SoundManager::play(TSoundID::EStomp);
}

void CGoomba::kickedFromUnderground()
{
    setSpeed(sf::Vector2f(0.f, -KBlockSize * 4.f));
    mCurrentAnimation = mFallingAnimation;
    mFallingTimer.start();
    setIsDead(true);
    setActiveDis(-1);
    SoundManager::play(TSoundID::EKick);
}

void CGoomba::killedByOther(CEnemy *other)
{
    kickedFromUnderground();
}

void CGoomba::killedByFireBall(CPlayerFireBall* fireBall)
{
   kickedFromUnderground();
}

void CGoomba::touchPlayer(CPlayer *player)
{
    player->getDamage();
}


TBoolean CGoomba::init(CProperties* properties)
{
    if(!CEnemy::init(properties)) return false;
    setActiveDis(KBlockSize * 15);
    sf::Texture* enemiesTexture = TextureManager::get(EEnemiesTextureID);
    if(!enemiesTexture) return false;
    mWalkingAnimation.create(enemiesTexture, { { 0,0,32,32 },{ 32,0,32,32 }});
    mFallingAnimation.create(enemiesTexture,{ 0,32,32,-32 });
    mCrampedAnimation.create(enemiesTexture, { 64,0,32,32 });
    mWalkingAnimation.setFrameDuration(15);
    mWalkingAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);
    mCurrentAnimation = mWalkingAnimation;
    setSpeed(sf::Vector2f(KBlockSize * -1.f, 0.f));
    mAnimationTicks = 0;
    setSize(KBlockSize, KBlockSize);

    mCrampingTimer = TTimer(180);
    mFallingTimer = TTimer(180);

    return true;
}
