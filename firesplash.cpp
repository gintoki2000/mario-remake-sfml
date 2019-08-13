#include "firesplash.h"
#include "texturemanager.h"
#include "utils.h"
CFireSplash *CFireSplash::create(const sf::Vector2f &position)
{
    CFireSplash* self = new (std::nothrow) CFireSplash();
    if(self && self->init(position))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CFireSplash::~CFireSplash()
{
    std::cout<<"~CFireSplash()"<<std::endl;
}

CFireSplash::CFireSplash()
{

}

TBoolean CFireSplash::init(const sf::Vector2f &position)
{
    if(!CParticle::init(15)) return false;
    sf::Texture* texture = TextureManager::get(TTextureID::EMarioTextureID);
    if(!texture) return false;
    mAnimation.create(texture, 31, 0, 16, 16, 3, 1);
    mAnimation.setFrameDuration(5);
    mAnimation.setPlayMode(TAnimation::TPlayMode::ELoop);
    setPosition(position);
    return true;
}

void CFireSplash::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform.combine(getTransform());
    renderTarget.draw(mAnimation.getCurrentKeyFrame(getFramesEslapsed()), renderStates);
}
