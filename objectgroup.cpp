#include "objectgroup.h"
#include "marioworld.h"
#include "player.h"
#include "camera.h"
CObjectGroup::CObjectGroup()
{

}

CObjectGroup::~CObjectGroup()
{

}

void CObjectGroup::update(TFloat deltaTime)
{
    if(isActive())
    {
        CMarioWorld* world = static_cast<CMarioWorld*>(getStage());
        TFloat playerX = world->getPlayer()->getPosition().x;
        for(const auto& child : getChildren())
        {
                TInt activeDis = static_cast<CGameObject*>(child)->getActiveDis();
                TFloat childX = child->getPosition().x;
                child->setActive((activeDis == -1) || (std::abs(childX - playerX < activeDis)));
        }
    }
    CNode::update(deltaTime);
}
/*
void CObjectGroup::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    if(isVisible())
    {
        renderStates.transform.combine(getTransform());
        Camera& camera = getStage()->getCamera();
        sf::FloatRect cameraBounds(camera.getLeft(), camera.getTop(), camera.getSize().x, camera.getSize().y);
        for(const auto& child : getChildren())
        {
            if(child->)
        }
    }
}*/
