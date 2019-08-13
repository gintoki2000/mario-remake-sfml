#include "platform.h"
#include "texturemanager.h"
void CPlatform::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform.combine(getTransform());
    renderTarget.draw(mSprite, renderStates);
}

CPlatform::CPlatform()
{

}

TBoolean CPlatform::init(CProperties *properties)
{
    if(!CItem::init(properties)) return false;
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mSprite.setTexture(*texture);
    mSprite.setTextureRect(sf::IntRect(0,0,95,15));
    setSize(sf::Vector2f(95, 15));
    return true;
}
