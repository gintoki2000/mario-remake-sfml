#include "shineparticle.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "texturemanager.h"

#include "utils.h"
CShineParticle *CShineParticle::create(const sf::Vector2f &position)
{
    CShineParticle* self = new (std::nothrow) CShineParticle();
    if(self && self->init(position))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CShineParticle::~CShineParticle()
{
    std::cout<<"~CShineParticle()"<<std::endl;
}

void CShineParticle::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform*=getTransform();
    renderTarget.draw(mAnimation.getCurrentKeyFrame(getFramesEslapsed()), renderStates);
}

CShineParticle::CShineParticle()
{

}

TBoolean CShineParticle::init(const sf::Vector2f &position)
{
    CParticle::init(15);
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mAnimation.create(texture, 0, 116, 40, 32, 5,1);
    mAnimation.setFrameDuration(15 / 5);
    setPosition(position);
    return true;
}
