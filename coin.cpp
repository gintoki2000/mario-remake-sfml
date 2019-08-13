#include "coin.h"
#include "soundmanager.h"
#include "texturemanager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "utils.h"
#include "stage.h"
#include "shineparticle.h"
#include "marioworld.h"
CCoin *CCoin::create(CProperties* properties)
{
    CCoin* self = new (std::nothrow) CCoin();
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CCoin *CCoin::createOnBlock(const sf::Vector2f &position)
{
    CCoin* self = new (std::nothrow) CCoin();
    if(self && self->init(position))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CCoin::~CCoin()
{
    std::cout<<"~CCoin()"<<std::endl;
}

void CCoin::onCollected(CPlayer *player)
{
    SoundManager::play(TSoundID::ECoin);
    CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
    world->addParticle(CShineParticle::create(getPosition() + KVecLeft * 4.f));
    remove();
}

void CCoin::update(TFloat deltaTime)
{
    mAnimTicks++;
    if(mAutoRemoveTimer.tick())
    {
        move(KVecUp * 4.f);
        if(mAutoRemoveTimer.isComplete())
        {
            SoundManager::play(TSoundID::ECoin);
            CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
            world->addParticle(CShineParticle::create(getPosition() + KVecLeft * 4.f));
            remove();
        }
    }
}

void CCoin::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform.combine(getTransform());
    renderTarget.draw(mAnim.getCurrentKeyFrame(mAnimTicks), renderStates);
}

CCoin::CCoin()
{

}

TBoolean CCoin::init(CProperties* properties)
{
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mAnim.create(texture, 0, 84, 32, 32,4, 1);
    mAnim.setPlayMode(TAnimation::TPlayMode::ELoop);
    mAnim.setFrameDuration(15);

    mAnimTicks = 0;
    auto x = properties->get<CFloat>("x");
    auto y = properties->get<CFloat>("y");
    if(x && y)
    {
        setPosition(x->value(), y->value());
    }else return false;
    setSize(32.f, 32.f);
    return true;
}

TBoolean CCoin::init(const sf::Vector2f &position)
{
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mAnim.create(texture, 0, 84, 32, 32,4, 1);
    mAnim.setPlayMode(TAnimation::TPlayMode::ELoop);
    mAnim.setFrameDuration(5);
    mAnimTicks = 0;
    setPosition(position);
    setSize(32.f, 32.f);
    mAutoRemoveTimer = TTimer(10);
    mAutoRemoveTimer.start();
    return true;
}
