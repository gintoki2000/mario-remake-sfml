#include "breakblockparticle.h"
#include "texturemanager.h"
#include "utils.h"
#include <SFML/Graphics/RenderTarget.hpp>
const TFloat CBreakBlockParticle::KGravityAcceleration = KBlockSize * 15.f;
CBreakBlockParticle *CBreakBlockParticle::create(const sf::Vector2f &position, const sf::Vector2f &diection)
{
    CBreakBlockParticle* self = new (std::nothrow) CBreakBlockParticle();
    if(self && self->init(position, diection))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CBreakBlockParticle::~CBreakBlockParticle()
{
    std::cout<<"~CBreakBlockParticle()"<<std::endl;
}

void CBreakBlockParticle::update(TFloat deltaTime)
{
    CParticle::update(deltaTime);
    mDirection += KVecDown * KGravityAcceleration * deltaTime;
    move(mDirection * deltaTime);
}

void CBreakBlockParticle::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform*=getTransform();
    renderTarget.draw(mAnimation.getCurrentKeyFrame(getFramesEslapsed()), renderStates);
}

CBreakBlockParticle::CBreakBlockParticle()
{

}

TBoolean CBreakBlockParticle::init(const sf::Vector2f &position, const sf::Vector2f &direction)
{
    if(!CParticle::init(60 * 3)) return false;
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mAnimation.create(texture, {{96, 0, 16, 16}, {96, 16, 16, -16}});
    mAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);
    mAnimation.setFrameDuration(3);
    mDirection = direction;
    setPosition(position);
    return true;
}
