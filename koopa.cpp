#include "koopa.h"
#include "texturemanager.h"
#include "player.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "soundmanager.h"
#include <cmath>
#include "property.h"
#include <stdexcept>
const sf::Vector2f CKoopa::KNormalSize = sf::Vector2f(32.f, 48.f);
const sf::Vector2f CKoopa::KShellSize = sf::Vector2f(KBlockSize, KBlockSize);
const TInt CKoopa::KHiddenTime = 5 * 60;
const TInt CKoopa::KDieTime = 3 * 60;
const TInt CKoopa::KClimbTime = 2 * 60;
const TFloat CKoopa::KJumpVel = KBlockSize * 7.f;
CKoopa *CKoopa::create(CProperties* properties)
{
    CKoopa* self = new (std::nothrow) CKoopa();
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CKoopa::~CKoopa()
{
    std::cout<<"~CKoopa()"<<std::endl;
}

void CKoopa::update(TFloat deltaTime)
{

    switch (mState) {
    case TState::EDie:
        updatePhysics(deltaTime);
        updateDirection();
        mStateTicks++;
        if(mStateTicks >= KDieTime)
        {
            remove();
        }
        break;
    case TState::ELevitating:
        mAnimationTicks++;
        setPosition(mInitPos + KVecUp * sinf(static_cast<TFloat>(mAnimationTicks) * 3.14f / 180.f) * KBlockSize);
        break;
    case TState::ENormal:
        mAnimationTicks++;
        updatePhysics(deltaTime);
        updateCollision(deltaTime);
        if(mCollisionStates & EFloorCollision) setSpeedY(0.f);
        if(mCollisionStates & (ELeftCollision | ERightCollision)) setSpeedX(-getSpeed().x);
        updateDirection();
        break;
    case TState::EHidingInShell:
        mStateTicks++;
        updatePhysics(deltaTime);
        updateCollision(deltaTime);
        if(mCollisionStates & EFloorCollision) setSpeedY(0.f);
        if(mCollisionStates & (ELeftCollision | ERightCollision)) setSpeedX(-getSpeed().x);
        updateDirection();
        if(mStateTicks >= KHiddenTime)
            setState(TState::EClimbing);
        break;
    case TState::ECrazyShell:
        mAnimationTicks++;
        updatePhysics(deltaTime);
        updateCollision(deltaTime);
        if(mCollisionStates & EFloorCollision) setSpeedY(0.f);
        if(mCollisionStates & (ELeftCollision | ERightCollision)) setSpeedX(-getSpeed().x);
        updateDirection();
        killOther();
        break;
    case TState::EClimbing:
        mAnimationTicks++;
        mStateTicks++;
        updatePhysics(deltaTime);
        updateCollision(deltaTime);
        if(mCollisionStates & EFloorCollision) setSpeedY(0.f);
        if(mCollisionStates & (ELeftCollision | ERightCollision)) setSpeedX(-getSpeed().x);
        if(mStateTicks >= KClimbTime)
            setState(TState::ENormal);
        break;
    case TState::EJumping:
        mAnimationTicks++;
        updatePhysics(deltaTime);
        updateCollision(deltaTime);
        mIsGounded = mCollisionStates & EFloorCollision;
        if(mIsGounded)
        {
            setSpeedY(0.f);
        }
        if(mCollisionStates & (ELeftCollision | ERightCollision)) setSpeedX(-getSpeed().x);
        if(mIsGounded)
        {
            addImpluse(KVecUp * KJumpVel);
        }
        updateDirection();
        break;
    }

}

void CKoopa::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform *= getTransform();

    const sf::Sprite* sprite = nullptr;
    switch (mState)
    {
    case TState::ENormal:
        sprite = &mWalkingAni.getCurrentKeyFrame(mAnimationTicks);
        break;
    case TState::ELevitating:
        sprite = &mFlyingAni.getCurrentKeyFrame(mAnimationTicks);
        break;
    case TState::EDie:
        sprite = mDylingSprite;
        break;
    case TState::ECrazyShell:
        sprite = &mCrazyShellAni.getCurrentKeyFrame(mAnimationTicks);
        break;
    case TState::EHidingInShell:
        sprite = &mShell;
        break;
    case TState::EClimbing:
        sprite = &mClimbAni.getCurrentKeyFrame(mAnimationTicks);
        break;
    case TState::EJumping:
        sprite = &mFlyingAni.getCurrentKeyFrame(mAnimationTicks);
        break;
    }
    if(!sprite)
        throw std::runtime_error("could not determine sprite will be draw! in function CKoopa::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)");
    if(mDirection == TDirection::ERight)
    {
        renderTarget.draw(flipSprite(*sprite), renderStates);
    }
    else
    {
        renderTarget.draw(*sprite, renderStates);
    }
}


void CKoopa::kickedByPlayer(CPlayer *player)
{

    switch (mState)
    {
    case TState::ENormal:
    case TState::ECrazyShell:
    {
        setState(TState::EHidingInShell);
        SoundManager::play(TSoundID::EStomp);
        break;
    }
    case TState::ELevitating:
    case TState::EJumping:
    {
        setState(TState::ENormal);
        SoundManager::play(TSoundID::EStomp);
        break;
    }
    case TState::EHidingInShell:
    case TState::EClimbing:
    {
        setState(TState::ECrazyShell);
        setSpeed((player->getSpeed().x > 0.f ? KVecRight : KVecRight) * KBlockSize * 7.f);
        SoundManager::play(TSoundID::EKick);
        break;
    }
    }
}

void CKoopa::kickedFromUnderground()
{
    setState(TState::EDie);
    SoundManager::play(TSoundID::EKick);
    mDylingSprite = &mFalling;
}

void CKoopa::killedByFireBall(CPlayerFireBall *fireBall)
{
    setState(TState::EDie);
    mDylingSprite = &mFallingShell;
    SoundManager::play(EKick);
}

void CKoopa::killedByOther(CEnemy *other)
{
    setState(TState::EDie);
    mDylingSprite=&mFallingShell;
    SoundManager::play(EKick);
}

void CKoopa::touchPlayer(CPlayer *player)
{
    if(mState == TState::EHidingInShell)
    {
        setState(TState::ECrazyShell);
        setSpeed((player->getDirection() & TDirection::ELeft ? KVecLeft : KVecRight) * KBlockSize * 7.f);
        SoundManager::play(EKick);
    }
    else if (mState != TState::EDie) {
        player->getDamage();
    }
}


CKoopa::CKoopa()
{

}

TBoolean CKoopa::init(CProperties* properties)
{
    CEnemy::init(properties);
    sf::Texture* enemiesTexture = TextureManager::get(TTextureID::EEnemiesTextureID);
    if(!enemiesTexture) return false;
    mWalkingAni.create(enemiesTexture, { { 0,32,32,48 },{ 32,32,32,48 } });
    mCrazyShellAni.create(enemiesTexture, 64, 48, 32, 32, 4, 1);
    mClimbAni.create(enemiesTexture, { { 64,48,32,32 },{ 192,48,32,32 }});
    mFlyingAni.create(enemiesTexture, 224, 32, 32, 48, 2, 1);

    mWalkingAni.setPlayMode(TAnimation::TPlayMode::ELoop);
    mCrazyShellAni.setPlayMode(TAnimation::TPlayMode::ELoop);
    mClimbAni.setPlayMode(TAnimation::TPlayMode::ELoop);
    mFlyingAni.setPlayMode(TAnimation::TPlayMode::ELoop);

    mWalkingAni.setFrameDuration(15);
    mCrazyShellAni.setFrameDuration(3);
    mClimbAni.setFrameDuration(20);
    mFlyingAni.setFrameDuration(10);

    mShell.setTexture(*enemiesTexture);
    mShell.setTextureRect(sf::IntRect(64, 48, 32, 32));

    mFalling.setTexture(*enemiesTexture);
    mFalling.setTextureRect(sf::IntRect(0, 80, 32, -48));

    mFallingShell.setTexture(*enemiesTexture);
    mFallingShell.setTextureRect(sf::IntRect(64, 48 + 32, 32, -32));

    mDirection = ELeft;
    mAnimationTicks = 0;
    mState = TState::ENone;
    mDylingSprite = nullptr;
    mIsGounded = false;

    setSize(KNormalSize.x, KNormalSize.y);

    setState(TState::ENormal);

    auto startingStatePro = properties->get<CString>("StartingState");
    if(startingStatePro)
    {
        if(startingStatePro->value() == "normal")
            setState(TState::ENormal);
        else if(startingStatePro->value() == "jumping")
            setState(TState::EJumping);
        else if(startingStatePro->value() == "levitating")
            setState(TState::ELevitating);
        else
        {
            std::cout<<"Starting state="<<startingStatePro->value()<<" What?(CKoopa::init)"<<std::endl;
        }
    }

    mInitPos = getPosition();
    return true;
}

void CKoopa::updateDirection()
{
    if(getSpeed().x < 0.f)
        mDirection = ELeft;
    else if(getSpeed().x > 0.f)
        mDirection = ERight;
}



void CKoopa::setState(CKoopa::TState state)
{
    if(mState == state) return;
    mState = state;
    switch (state)
    {
        case TState::ENormal:
        setSpeed((mDirection & ERight ? KVecRight : KVecLeft) * KBlockSize * 1.f);
        setSize(KNormalSize);
        mAnimationTicks = 0;
        break;
    case TState::EHidingInShell:
        setSpeed(KVecZero);
        setSize(KShellSize);
        mStateTicks = 0;
        break;
    case TState::ELevitating:
        mAnimationTicks = 0;
        setSize(KNormalSize);
        break;
    case TState::EDie:
        setSpeed(sf::Vector2f(0.f, -KBlockSize * 5.f));
        mStateTicks = 0;
        setIsDead(true);
        break;
    case TState::ECrazyShell:
        mAnimationTicks = 0;
        setSize(KShellSize);
        break;
    case TState::EClimbing:
        mAnimationTicks = 0;
        mStateTicks = 0;
        setSize(KShellSize);
        break;
    case TState::EJumping:
        mAnimationTicks = 0;
        setSize(KNormalSize);
        break;
    }
}
