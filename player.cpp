#include "player.h"
#include "mainstate.h"
#include "marioworld.h"
#include "texturemanager.h"
#include "mariogame.h"
#include "keybinding.h"
#include <algorithm>
#include <SFML/Graphics/RectangleShape.hpp>
#include "enemy.h"
#include "block.h"
#include "soundmanager.h"
#include "playerfireball.h"
#include "property.h"
#include "pickup.h"
#include "item.h"
#include "platform.h"
#include "portal.h"
CPlayer::CPlayer():
    KSuperSize(32.f, 64.f),
    KNormalSize(32.f, 32.f),
    KSitingSize(32.f , 44.f)
{
}

CPlayer *CPlayer::create(CProperties* properties)
{
    CPlayer* self = new (std::nothrow) CPlayer();
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CPlayer::~CPlayer()
{
    std::cout<<"~CPlayer()"<<std::endl;
}

void CPlayer::update(TFloat deltaTime)
{
    if(!mWorld)
    {
        mWorld = static_cast<CMarioWorld*>(getStage());
        mTiledMap = mWorld->getTiledMap();
    }
    if(mDylingTimer.tick())
    {

        addImpluse(KVecDown * KGravityAcceleration * deltaTime);
        move(mSpeed * deltaTime);
        mAnimationState = EDyling;
        if(mDylingTimer.isComplete())
        {


        }
        return;
    }

    if(mEnteringPortalTimer.tick())
    {
        mAnimationTicks++;
        TFloat ownTop = getY();
        TFloat ownBottom = getY() + getHeight();
        TFloat ownLeft = getX();
        TFloat ownRight = getX() + getWidth();

        TFloat portalBottom = mPortal->getY() + mPortal->getHeight();
        TFloat portalLeft = mPortal->getX();
        TFloat portalRight = mPortal->getX() + mPortal->getWidth();
        if(mPortal->getDirection() & EUp)
        {
            mAnimationState = EStanding;
            move(KVecUp * 0.5f);
            if(ownBottom > portalBottom)
                mEnteringPortalTimer.finish();

        }
        else if(mPortal->getDirection() & EDown)
        {
            mAnimationState = EStanding;
            move(KVecDown * 0.5f);
            if(ownTop > portalBottom)
                mEnteringPortalTimer.finish();
        }
        else if(mPortal->getDirection() & ELeft)
        {
            mAnimationState = EWalking;
            move(KVecLeft * 0.5f);
            if(ownRight < portalLeft)
                mEnteringPortalTimer.finish();
        }
        else if(mPortal->getDirection() & ERight)
        {
            mAnimationState = EWalking;
            move(KVecRight * 0.5f);
            if(ownLeft > portalRight )
                mEnteringPortalTimer.finish();
        }

        if(mEnteringPortalTimer.isComplete())
        {
            TString worldFile = "resources/worlds/" + mPortal->getWorld();
            auto session = mWorld->getState()->getGame()->getSession();
            session->set("powerUpState", new CProperty<TPowerUpState>(mPowerUpState));
            mWorld->getState()->LoadNewWorld(worldFile, mPortal->getSpawningPortal());
        }
        return;
    }
    if(mExitingPortalTimer.tick())
    {
        mAnimationTicks++;
        TFloat ownTop = getY();
        TFloat ownBottom = getY() + getHeight();
        TFloat ownLeft = getX();
        TFloat ownRight = getX() + getWidth();

        TFloat portalTop = mPortal->getY();
        TFloat portalBottom = mPortal->getY() + mPortal->getHeight();
        TFloat portalLeft = mPortal->getX();
        TFloat portalRight = mPortal->getX() + mPortal->getWidth();
        if(mPortal->getDirection() & EUp)
        {
            mAnimationState = EStanding;
            move(KVecUp * 0.5f);
            if(ownBottom < portalBottom)
                mExitingPortalTimer.finish();

        }
        else if(mPortal->getDirection() & EDown)
        {
            mAnimationState = EStanding;
            move(KVecDown * 0.5f);
            if(ownTop > portalTop)
                mExitingPortalTimer.finish();
        }
        else if(mPortal->getDirection() & ELeft)
        {
            mAnimationState = EWalking;
            move(KVecLeft * 0.5f);
            if(ownRight < portalLeft)
                mExitingPortalTimer.finish();
        }
        else if(mPortal->getDirection() & ERight)
        {
            mAnimationState = EWalking;
            move(KVecRight * 0.5f);
            if(ownLeft > portalRight )
                mExitingPortalTimer.finish();
        }

        if(mExitingPortalTimer.isComplete())
        {
            toBack();
            mSuperAnimations[EWalking].setFrameDuration(15);
            mNormalAnimations[EWalking].setFrameDuration(15);
        }
        return;
    }
    if(mSlipingTimer.tick())
    {
        mAnimationState = TAnimationState::ESliping;

        //on leave state
        if(mSlipingTimer.isComplete())
        {
            mSpeed.x = 0.f;
        }
        return;
    }

    if(mDemotingTimer.tick())
    {
        if(mPowerUpState == TPowerUpState::ESuper)
        {
            mAnimationState = EStanding;
            setVisible((mDemotingTimer.getFramesEslapsed() / 3) % 2);
        }
        else if(mPowerUpState == TPowerUpState::ENormal)
        {
            mAnimationState = EDemoting;
        }
        if(mDemotingTimer.isComplete())
        {
            mProtectionTimer.start();
            if(mPowerUpState == TPowerUpState::ENormal)
            {
                setSize(KNormalSize);
            }
            static_cast<CMarioWorld*>(getStage())->activateScene();
        }
        return;
    }
    if(mPromotingTimer.tick())
    {
        if(mPowerUpState == TPowerUpState::EFire)
        {

        }
        else if(mPowerUpState == TPowerUpState::ESuper)
        {
            mAnimationState = TAnimationState::EGrowing;
            setSize((mPromotingTimer.getFramesEslapsed() / mGrowingAnimation.getFrameDuration()) % 2 ? KNormalSize : KSuperSize);
        }
        if(mPromotingTimer.isComplete())
        {
            setSize(KSuperSize);
            static_cast<CMarioWorld*>(getStage())->activateScene();

        }
        return;
    }

    if(mIsLeverClear)
    {

        return;
    }

    if(mProtectionTimer.tick())
    {
        setVisible((mProtectionTimer.getFramesEslapsed() / 6) % 2);
        if(mProtectionTimer.isComplete())
        {
            show();
        }
    }

    mFiringTimer.tick();
    mJumpingTimer.tick();

    mAnimationTicks++;
    inputProcess(deltaTime);

    physicalProcess(deltaTime);

    collisionProcess(deltaTime);

    kickBlockProcess(deltaTime);

    animationProcess(deltaTime);

}



void CPlayer::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    sf::RenderStates parentStates = renderStates;
    renderStates.transform.combine( getTransform() );

    if(mAnimationState == TAnimationState::EDyling)
    {
        renderTarget.draw(mDylingAnimation.getCurrentKeyFrame(mAnimationTicks), renderStates);
    }
    else if(mAnimationState == TAnimationState::EFiring)
    {

        const auto& sprite = mFiringAnimation.getCurrentKeyFrame(mFiringTimer.getFramesEslapsed());
        if(mDirection & ELeft)
        {
            renderTarget.draw(flipSprite(sprite), renderStates);
        }
        else if(mDirection & ERight)
        {
            renderTarget.draw(sprite, renderStates);
        }
    }
    else if(mAnimationState == TAnimationState::EGrowing)
    {

        renderTarget.draw(mGrowingAnimation.getCurrentKeyFrame(mPromotingTimer.getFramesEslapsed()), renderStates);
    }
    else if(mAnimationState == TAnimationState::EDemoting)
    {
        renderTarget.draw(mDemotingAnimation.getCurrentKeyFrame(mDemotingTimer.getFramesEslapsed()), renderStates);
    }
    else
    {

        if(mPowerUpState == TPowerUpState::ENormal)
        {
            const auto& sprite = mNormalAnimations[mAnimationState].getCurrentKeyFrame(mAnimationTicks);
            if(mDirection & ELeft)
            {
                renderTarget.draw(flipSprite(sprite), renderStates);
            }
            else if(mDirection & ERight)
            {
                renderTarget.draw(sprite, renderStates);
            }
        }else
        {
            const auto& sprite = mSuperAnimations[mAnimationState].getCurrentKeyFrame(mAnimationTicks);
            if(mDirection & ELeft)
            {
                renderTarget.draw(flipSprite(sprite), renderStates);
            }
            else if(mDirection & ERight)
            {
                renderTarget.draw(sprite, renderStates);
            }
        }
    }

    CNode::draw(renderTarget, parentStates);
}



void CPlayer::getDamage()
{
    if(!mProtectionTimer.isActive())
    {
        if(mPowerUpState != TPowerUpState::ENormal && mIsSiting)
        {
            setSize(KSuperSize);
            mIsSiting = false;
        }
        switch (mPowerUpState) {
        case CPlayer::TPowerUpState::EFire:
            mPowerUpState = TPowerUpState::ESuper;
            SoundManager::play(TSoundID::EPipe);
            mDemotingTimer.start();
            mAnimationState = TAnimationState::EDemoting;
            mWorld->deactiveScene();
            break;
        case CPlayer::TPowerUpState::ESuper:
            mPowerUpState = TPowerUpState::ENormal;
            SoundManager::play(TSoundID::EPipe);
            mDemotingTimer.start();
            mAnimationState = TAnimationState::EDemoting;
            mWorld->deactiveScene();
            break;
        case CPlayer::TPowerUpState::ENormal:
            mDylingTimer.start();
            mIsDead = true;
            mWorld->deactiveScene();
            setSpeed(sf::Vector2f(KBlockSize, -KBlockSize * 8.f));
            SoundManager::play(TSoundID::EMarioDie);
            mAnimationState = EDyling;
            break;
        }
    }
}

void CPlayer::enterPortal(CPortal *portal)
{
    mPortal = portal;
    mEnteringPortalTimer.start();
    toFront();
    mSuperAnimations[EWalking].setFrameDuration(15);
    mNormalAnimations[EWalking].setFrameDuration(15);
    if(portal->getDirection() & EUp)
    {
        setPosition(portal->getX() + (portal->getWidth() - getWidth()) /2.f, portal->getY());
    }
    else if(portal->getDirection() & EDown)
    {
        setPosition(portal->getX() + (portal->getWidth() - getWidth()) / 2.f, portal->getY() + portal->getHeight() - getHeight());
    }
    else if(portal->getDirection() & ELeft)
    {
        setPosition(getX(), portal->getY() + portal->getHeight() - getHeight());
    }
    else if(portal->getDirection() & ERight)
    {
        setPosition(getX(), portal->getY() + portal->getHeight() - getHeight());
    }
    SoundManager::play(EPipe);
}

void CPlayer::exitPortal(CPortal *portal)
{
    mPortal = portal;
    mExitingPortalTimer.start();
    toFront();
    if(portal->getDirection() & EUp)
    {
        setPosition(portal->getX() + (portal->getWidth() - getWidth()) /2.f, portal->getY() + portal->getHeight());
    }
    else if(portal->getDirection() & EDown)
    {
        setPosition(portal->getX() + (portal->getWidth() - getWidth()) / 2.f, portal->getY() - getHeight());
    }
    else if(portal->getDirection() & ELeft)
    {
        setPosition(portal->getX() + portal->getWidth(), portal->getY() + portal->getHeight() - getHeight());
    }
    else if(portal->getDirection() & ERight)
    {
        setPosition(portal->getX() - getWidth(), portal->getY() + portal->getHeight() - getHeight());
    }

}

void CPlayer::setSpeedX(TFloat speedX)
{
    mSpeed.x = speedX;
}

void CPlayer::setSpeedY(TFloat speedY)
{
    mSpeed.y = speedY;
}

const TFloat &CPlayer::getSpeedX()
{
    return mSpeed.x;
}

const TFloat &CPlayer::getSpeedY()
{
    return mSpeed.y;
}

TBoolean CPlayer::isSmall() const
{
    return mPowerUpState == TPowerUpState::ENormal;
}

TBoolean CPlayer::isDead() const
{
    return mIsDead;
}

TBoolean CPlayer::isFireMario() const
{
    return mPowerUpState == TPowerUpState::EFire;
}

void CPlayer::inputProcess(TFloat deltaTime)
{
    TBoolean isButtonAPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_A));
    TBoolean isButtonBPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_B));
    TBoolean isButtonLeftPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_LEFT));
    TBoolean isButtonRightPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_RIGHT));
    TBoolean isButtonUpPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_UP));
    TBoolean isButtonDownPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(EBUTTON_DOWN));
    if(!isButtonLeftPressed && !isButtonRightPressed && mIsGrounding)
    {
        mSpeed.x *= 0.8f;
    }
    mInputDirection = ENone;
    if(isButtonUpPressed) mInputDirection |= EUp;
    if(isButtonDownPressed) mInputDirection |= EDown;
    if(isButtonLeftPressed) mInputDirection |= ELeft;
    if(isButtonRightPressed) mInputDirection |= ERight;

    if(!mIsClimbing)
    {
        if(!mIsSiting || mIsJumping)
        {
            TFloat k = mIsJumping ? 0.7f : 1.f;
            if(isButtonLeftPressed)
            {
                if(getSpeed().x > KMaxWalkingSpeed * 0.75f && mIsGrounding)
                {
                    mSlipingTimer.start();
                    mSpeed.x = 0.f;
                }
                else addImpluse(KVecLeft * KWalkingAcceleration * k * deltaTime);
            }
            else if(isButtonRightPressed)
            {
                if(getSpeed().x < KMaxWalkingSpeed * -0.75f && mIsGrounding)
                {
                    mSlipingTimer.start();
                    mSpeed.x = 0.f;
                }
                else addImpluse(KVecRight * KWalkingAcceleration * k * deltaTime);
            }
        }

        if(mIsGrounding & !isSmall())
        {
            if(isButtonDownPressed && !mIsSiting)
            {
                mIsSiting = true;
                mSpeed.x  = 0.f;
                setSize(KSitingSize);
            }
            else if(isButtonUpPressed && mIsSiting)
            {
                mIsSiting = false;

                setSize(KSuperSize);
            }
        }

        if(mIsSiting)
        {
            if(isButtonLeftPressed)
            {
                mDirection |= ELeft;
                mDirection |= ~ERight;
            }
            else if(isButtonRightPressed)
            {
                mDirection |= ERight;
                mDirection |= ~ELeft;
            }
        }

        if(isButtonBPressed && !mFiringTimer.isActive() && !mIsSiting && isFireMario())
        {
            mFiringTimer.start();
            CPlayerFireBall* fireBall = CPlayerFireBall::create(mDirection);
            if(fireBall)
            {

                getStage()->getRoot()->attachChild(fireBall);
                fireBall->setPosition(localToStageCoords(KVecZero));
                fireBall->move(getSize() / 2.f);
            }
            SoundManager::play(EFireBall);
        }
    }


    if(isButtonAPressed && !mIsSiting)
    {
        if(mIsGrounding)
        {
            mIsJumping = true;
            mIsGrounding = false;
            mIsClimbing = false;
            mJumpingTimer.start();
            SoundManager::play(EJumpSuper);
            addImpluse(KVecUp * KJumpingForce);
        }else if(mJumpingTimer.tick())
        {
            addImpluse(KVecUp * KJumpingForce * static_cast<TFloat>(mJumpingTimer.getFramesRemaining() / mJumpingTimer.getTotalFrames()));
        }
    }

}

void CPlayer::collisionProcess(TFloat deltaTime)
{
    //reset collision states
    mCollisionStates = ENoCollision;

    sf::Vector2f deltaMove = getSpeed() * deltaTime;

    sf::Vector2f moveDis;

    move(deltaMove);
    mCollisionStates = mTiledMap->checkCollisison(getPosition(), getSize(), deltaMove, moveDis);
    move(moveDis);

    mIsGrounding = mCollisionStates & EFloorCollision;
    if(mIsGrounding)
    {
        mIsJumping = false;
        setSpeedY(0.f);
    }


    const std::vector<CNode*>& items = mWorld->getItems()->captureChildren();
    for(std::size_t i = 0; i < items.size(); i++)
    {
        CItem* item = static_cast<CItem*>(items[i]);
        TInt itemCollisionStates = item->checkCollisionWithPlayer(this);
        mCollisionStates |= itemCollisionStates;

        //jump-off
        if((itemCollisionStates & EFloorCollision) && (mInputDirection & EDown) && item->isTypeOf<CPlatform>())
        {
            move(0.f, item->getSize().y + 2);
        }

        if(itemCollisionStates & EFloorCollision)
        {
            mIsGrounding = true;
            mIsJumping = false;
        }
    }


    if(mCollisionStates & ECeilCollision) mSpeed.y = -0.1f * mSpeed.y;
    if(mCollisionStates & ELeftCollision) mSpeed.x = 0.f;
    if(mCollisionStates & ERightCollision) mSpeed.x = 0.f;



    sf::FloatRect playerBounds(getBounds());
    //check collision with enemies
    const std::vector<CNode*>& enemies = mWorld->getEnemies()->captureChildren();
    sf::Vector2f delta, intersection;
    for(std::size_t i = 0; i < enemies.size(); i++)
    {
        CEnemy* enemy = static_cast<CEnemy*>(enemies[i]);
        if(!(enemy->isDead()))
        {
            sf::FloatRect enemyBounds(enemy->getBounds());
            getIntersection(enemyBounds, playerBounds, delta, intersection);

            if(intersection.x < 0.f && intersection.y < 0.f)
            {

                if(delta.y < 0.f  && (-intersection.y <= (enemyBounds.height / 4.f)))
                {
                    enemy->kickedByPlayer(this);
                    setSpeedY(-0.5f * KJumpingForce);
                }
                else
                {
                    enemy->touchPlayer(this);
                }
            }
        }
    }
    //check collision with pickups
    const std::vector<CNode*>& pickUps = mWorld->getPickUps()->captureChildren();
    for(const auto& node : pickUps)
    {
        CPickUp* pickUp = static_cast<CPickUp*>(node);
        if(playerBounds.intersects(pickUp->getBounds()))
        {
            pickUp->onCollected(this);
        }
    }

    const std::vector<CNode*>& portals = mWorld->getPortals()->getChildren();
    for(std::size_t i = 0; i < portals.size(); i++)
    {
        CPortal* portal = static_cast<CPortal*>(portals[i]);
        if((portal->getType() == CPortal::TType::EEnter) && (mInputDirection & portal->getDirection()))
        {
            if(portal->getBounds().intersects(playerBounds))
            {
                enterPortal(portal);
                break;
            }
        }

    }

}

void CPlayer::physicalProcess(TFloat deltaTime)
{
    if(mIsClimbing) return;
    if(!mIsGrounding)
        addImpluse(KVecDown * KGravityAcceleration * deltaTime);

    if(std::abs(getSpeedX()) < 1.f) setSpeedX(0.f);
    setSpeedX( math::clamp(-KMaxWalkingSpeed, KMaxWalkingSpeed, getSpeedX()));

    if(getSpeedX() < 0.f)
    {
        mDirection = ELeft;
        mDirection |= ~ERight;
    }
    else if(getSpeedX() > 0.f)
    {
        mDirection = ERight;
        mDirection |= ~ELeft;
    }


}

void CPlayer::animationProcess(TFloat deltaTime)
{
    if(mFiringTimer.isActive())
    {
        mAnimationState = EFiring;
    }
    else if(mIsJumping)
    {
        mAnimationState = EJumping;
    }
    else if(mIsSiting)
    {
        mAnimationState = ESiting;
    }
    else if(mIsGrounding)
    {
        if(getSpeed().x != 0.f)
        {
            mAnimationState = EWalking;
        }
        else
        {
            mAnimationState = EStanding;
        }
    }
    //std::cout<<(mDirection == TDirection::ELeft ? "left" : "right")<<std::endl;
}

void CPlayer::kickBlockProcess(TFloat deltaTime)
{
    if(mCollisionStates & ECeilCollision)
    {
        sf::Vector2f playerTopLeft = getPosition();
        sf::Vector2f playerTopRight(playerTopLeft.x + getSize().x, playerTopLeft.y);

        playerTopLeft.y -= 16.f;
        playerTopRight.y -= 16.f;

        CBlock* blockTopLeft = mTiledMap->getBlock(mTiledMap->pixelToBlockCorrds(playerTopLeft));
        CBlock* blockTopRight = mTiledMap->getBlock(mTiledMap->pixelToBlockCorrds(playerTopRight));

        //both left and right block exist kick midle block
        if(blockTopLeft && blockTopRight)
        {
            sf::Vector2f playerCenter(playerTopLeft.x + getSize().x / 2.f, playerTopLeft.y);
            CBlock* blockCenter = mTiledMap->getBlock(mTiledMap->pixelToBlockCorrds(playerCenter));
            if(blockCenter)
            {
                blockCenter->kicked(this);
            }
            else if(mDirection & ELeft)
            {
                blockTopLeft->kicked(this);
            }
            else if(mDirection & ERight)
            {
                blockTopRight->kicked(this);
            }
        }
        else if(blockTopLeft)
        {
            blockTopLeft->kicked(this);
        }
        else if(blockTopRight)
        {
            blockTopRight->kicked(this);
        }
    }
}

TInt CPlayer::getDirection() const
{
    return mDirection;
}

void CPlayer::standUp()
{
    mIsSiting = false;
    setSize(KSuperSize);
}

void CPlayer::sitDown()
{
    mIsSiting = true;
    setSize(KSitingSize);
    setSpeedX(0.f);
}

const sf::Vector2f &CPlayer::getSpeed() const
{
    return mSpeed;
}

void CPlayer::setSpeed(const sf::Vector2f &speed)
{
    mSpeed = speed;
}

void CPlayer::addImpluse(const sf::Vector2f &impluse)
{
    mSpeed += impluse;
}

void CPlayer::promote()
{
    if(mPowerUpState == TPowerUpState::ENormal)
    {
        mPromotingTimer.start();
        mPowerUpState = TPowerUpState::ESuper;
        mWorld->deactiveScene();
        SoundManager::play(EPowerUp);
    }
    else if(mPowerUpState == TPowerUpState::ESuper)
    {
        mPromotingTimer.start();
        mPowerUpState = TPowerUpState::EFire;
        mWorld->deactiveScene();
        SoundManager::play(EPowerUp);
    }
    else if(mPowerUpState == TPowerUpState::EFire)
    {

    }
}

TBoolean CPlayer::init(CProperties *properties)
{

    CGameObject::init(properties);
    sf::Texture* texture = TextureManager::get(TTextureID::EMarioTextureID);
    if(!texture){ std::cout<<"there no texture(CPlayer::init)"<<std::endl; return false;}
    //init animations

    mNormalAnimations[EStanding].create(texture, {0, 96, 32, 32});
    mNormalAnimations[EWalking].create(texture, 32, 96, 32, 32, 3, 1);
    mNormalAnimations[EJumping].create(texture, {160, 96, 32, 32});
    mNormalAnimations[ESwiming].create(texture, 288, 96, 32, 32, 3, 1);
    mNormalAnimations[ESliping].create(texture, {130, 96, 32, 29});
    mNormalAnimations[EClimbing].create(texture, 224, 96, 32, 32, 2, 1);
    mNormalAnimations[ESiting].create(texture, {192, 96, 32, 32});

    mNormalAnimations[EWalking].setFrameDuration(10);
    mNormalAnimations[ESwiming].setFrameDuration(15);

    mNormalAnimations[EWalking].setPlayMode(TAnimation::TPlayMode::ELoop);
    mNormalAnimations[ESwiming].setPlayMode(TAnimation::TPlayMode::ELoop);





    mSuperAnimations[EStanding].create(texture, sf::IntRect(0, 32, 32, 64));
    mSuperAnimations[EWalking].create(texture, 32, 32, 32, 64, 3, 1);
    mSuperAnimations[EJumping].create(texture, sf::IntRect(160, 32, 32, 64));
    mSuperAnimations[ESliping].create(texture, sf::IntRect(128, 32, 32, 64));
    mSuperAnimations[ESiting].create(texture, sf::IntRect(192, 52, 32, 44));
    mSuperAnimations[EClimbing].create(texture, 256, 32, 32, 64, 2, 1);
    mSuperAnimations[ESwiming].create(texture, 321, 32, 40, 64, 3, 1);

    mSuperAnimations[EWalking].setPlayMode(TAnimation::TPlayMode::ELoop);
    mSuperAnimations[ESwiming].setPlayMode(TAnimation::TPlayMode::ELoop);

    mSuperAnimations[EWalking].setFrameDuration(10);
    mSuperAnimations[ESwiming].setFrameDuration(15);

    mDylingAnimation.create(texture, sf::IntRect(192, 96, 32, 32));
    mFiringAnimation.create(texture, {{ 0,32,32,64 },{ 224,32,32,64 }});
    mGrowingAnimation.create(texture, {sf::IntRect(0, 32, 32, 64), sf::IntRect(0, 96, 32, 32)});
    mDemotingAnimation.create(texture, { { 401,32,40,64 },{ 288,96,32,32 } });

    mFiringAnimation.setFrameDuration(5);
    mGrowingAnimation.setFrameDuration(6);
    mDemotingAnimation.setFrameDuration(6);

    mGrowingAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);
    mDemotingAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);

    mDylingTimer = TTimer(180);
    mPromotingTimer = TTimer(60);
    mDemotingTimer = TTimer(60);
    mEnteringPortalTimer = TTimer(60 * 5);
    mExitingPortalTimer = TTimer(60 * 5);
    mSlipingTimer = TTimer(15);

    mFiringTimer = TTimer(30);
    mJumpingTimer = TTimer(100);
    mProtectionTimer = TTimer(60 * 4);

    mWorld = nullptr;
    mTiledMap = nullptr;

    CProperties* gameSession = properties->get<CProperty<CProperties*>>("session")->value();

    mPowerUpState = gameSession->get<CProperty<TPowerUpState>>("powerUpState")->value();

    if( mPowerUpState == TPowerUpState::ENormal)
    {
        setSize(KNormalSize.x, KNormalSize.y);
    }
    else if(mPowerUpState == TPowerUpState::ESuper)
    {
        setSize(KSuperSize.x, KSuperSize.y);
    }
    else if(mPowerUpState == TPowerUpState::EFire)
    {
        setSize(KSuperSize.x, KSuperSize.y);
    }

    mAnimationState = TAnimationState::EStanding;
    mDirection = TDirection::ERight;

    mIsGrounding = false;
    mIsSiting = false;
    mIsJumping = false;
    mIsClimbing = false;
    mIsDead = false;
    mIsLeverClear = false;

    mCollisionStates = TCollisionState::ENoCollision;

    mAnimationTicks = 0;
    setSpeed(KVecZero);

    return true;
}
