#include "jumper.h"
#include "texturemanager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "player.h"
CJumper *CJumper::create(CProperties *properties)
{
    CJumper* self = new (std::nothrow) CJumper;
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

void CJumper::update(TFloat deltaTime)
{
    if(mBouncingTimer.tick() && mBouncingTimer.isComplete())
    {
        setSize(sf::Vector2f(32, 48));
    }
}

void CJumper::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform.combine(getTransform());
    if(mBouncingTimer.isActive())
    {
        renderTarget.draw(mBouncingAnim.getCurrentKeyFrame(mBouncingTimer.getFramesEslapsed()), renderStates);
        return;
    }
    renderTarget.draw(mNormalSprite, renderStates);
}

TInt CJumper::checkCollisionWithPlayer(CPlayer *player)
{
    if(mBouncingTimer.isActive()) return ENoCollision;
    sf::Vector2f delta;
    sf::Vector2f intersection;
    sf::FloatRect playerBounds = player->getBounds();
    sf::FloatRect ownBounds = getBounds();

    getIntersection(ownBounds, playerBounds, delta, intersection);
    if(intersection.x < 0.f && intersection.y < 0.f)
    {
        TBoolean isPlayerOnTop = (playerBounds.top + playerBounds.height) < (ownBounds.top + ownBounds.height);
        if(isPlayerOnTop && (player->getSpeed().y > (KBlockSize * 10.f)))
        {
            player->setSpeed(KVecUp * KBlockSize * 8.f);
            mBouncingTimer.start();
            setSize(sf::Vector2f(32, 64));
            return  EFloorCollision;
        }
        else
        {

            if(delta.y > 0)
            {
                player->move(0.f, -intersection.y);
                return ECeilCollision;
            }
            else
            {
                player->move(0.f, intersection.y);
                player->setSpeedY(0.f);
                return EFloorCollision;
            }

        }
    }
    return ENoCollision;
}

CJumper::CJumper()
{

}

TBoolean CJumper::init(CProperties *properties)
{
    if(!CItem::init(properties)) return false;
    sf::Texture* texture = TextureManager::get(TTextureID::EItemsTextureID);
    if(!texture) return false;
    mNormalSprite.setTexture(*texture);
    mNormalSprite.setTextureRect(sf::IntRect(32, 36, 32, 48));
    setSize(32, 48);
    mBouncingAnim.create(texture, 0, 20, 32, 64, 3, 1);
    mBouncingAnim.setPlayMode(TAnimation::TPlayMode::EReversed);
    mBouncingAnim.setFrameDuration(10);
    mBouncingTimer = TTimer(30);
    return true;
}
