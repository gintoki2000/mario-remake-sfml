#include "playerfireball.h"
#include "texturemanager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "marioworld.h"
#include "enemy.h"
#include "firesplash.h"
const TFloat CPlayerFireBall::KGravityAcceleration = KBlockSize * 20.f;
const TFloat CPlayerFireBall::KRecoilSpeed = KBlockSize * 8.f;
CPlayerFireBall::CPlayerFireBall()
{

}

TBoolean CPlayerFireBall::init(TInt direction)
{
    sf::Texture* marioTexture = TextureManager::get(EMarioTextureID);
    if(!marioTexture) return false;
    mAnimation.create(marioTexture, { { 0,0,16,16 }, { 16,0,16,16 }, { 16,0,-16,16 }, { 16,16,16,-16 }});
    mAnimation.setFrameDuration(3);
    mAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);
    mTimer = TTimer(240);
    mTimer.start();
    setSize(sf::Vector2f(16.f, 16.f));
    mAnimationTicks = 0;
    mTiledMap = nullptr;
    mSpeed.x = direction & ERight ? KRecoilSpeed : -KRecoilSpeed;
    return true;
}

CPlayerFireBall *CPlayerFireBall::create(TInt direction)
{
    CPlayerFireBall* self = new (std::nothrow) CPlayerFireBall();
    if(self && self->init(direction))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CPlayerFireBall::~CPlayerFireBall()
{
    std::cout<<"~CPlayerFireBall()"<<std::endl;
}

void CPlayerFireBall::update(TFloat deltaTime)
{
    if(mTimer.tick())
    {
        mAnimationTicks++;
        if(!mTiledMap)
        {
            mTiledMap = static_cast<CMarioWorld*>(getStage())->getTiledMap();
        }

        //<<<
        mSpeed += KVecDown * KGravityAcceleration * deltaTime;
        move(mSpeed * deltaTime);

        //<<<
        mCollisionStates = ENoCollision;
        move(mTiledMap->checkCollision(localToNodeCoords(mTiledMap, KVecZero), getSize(), getSpeed(), deltaTime, mCollisionStates));

        if(mCollisionStates & EFloorCollision)
        {
            mSpeed.y = -KRecoilSpeed;
        }

        if(mCollisionStates & (ELeftCollision | ERightCollision))
        {
            mSpeed.x = -mSpeed.x;
        }
        CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
        sf::FloatRect ownBounds(localToNodeCoords(world->getEnemies(), KVecZero), getSize());
        for(const auto& node : static_cast<CMarioWorld*>(getStage())->getEnemies()->captureChildren())
        {
            CEnemy* enemy = static_cast<CEnemy*>(node);
            if(!enemy->isDead() && enemy->getBounds().intersects(ownBounds))
            {
                enemy->killedByFireBall(this);
                world->addParticle(CFireSplash::create(getPosition()));
                remove();
            }
        }
    }else
    {
        auto fireSplash = CFireSplash::create(getPosition());
        if(fireSplash) getStage()->getRoot()->attachChild(fireSplash);
        remove();
    }
}

void CPlayerFireBall::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform *= getTransform();
    renderTarget.draw(mAnimation.getCurrentKeyFrame(mAnimationTicks), renderStates);
}

void CPlayerFireBall::setSpeed(const sf::Vector2f &speed)
{
    mSpeed = speed;
}

const sf::Vector2f &CPlayerFireBall::getSpeed() const
{
    return mSpeed;
}
