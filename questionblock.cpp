#include "questionblock.h"

#include "tiledmap.h"

#include "marioworld.h"

#include <iostream>

#include "property.h"


#include "shineparticle.h"

#include "utils.h"

#include "coin.h"

#include "mushroom.h"

#include "player.h"

#include "flower.h"
CQuestionBlock::CQuestionBlock()
{

}

TBoolean CQuestionBlock::init(const sf::Vector2f &position, CTile *tile)
{
    if(!CBlock::init(position, tile)) return false;
    setRigid(true);
    if(!tile)
    {
        std::cout<<"tile can not be nullptr! in function(CBrickBlock::init)!"<<std::endl;
        return false;
    }
    auto spawnItemProperty = tile->getProperties()->get<CString>("SpawnItemType");
    if(!spawnItemProperty)
    {
        std::cout<<"there is no SpawnItemType property! in function(CQuestionBlock::init)!"<<std::endl;
        return false;
    }
    mSpawnItem = spawnItemProperty->value();
    mIsSpawned = false;
    mKickedSpeed = 0.f;
    mKickedValue = 0.f;
    return true;
}

CQuestionBlock *CQuestionBlock::create(const sf::Vector2f &position, CTile *tile)
{
    CQuestionBlock* self = new (std::nothrow) CQuestionBlock();
    if(self && self->init(position, tile))
    {
        return self;
    }
    delete self;
    return nullptr;
}

void CQuestionBlock::update(TFloat deltaTime)
{


    if(mKickedSpeed != 0.f)
    {
        mKickedSpeed += -mKickedValue * 32.f  * deltaTime;
        mKickedValue += mKickedSpeed * deltaTime;

        if(mKickedValue > 0.f)
        {
            mKickedSpeed = 0.f;
            mKickedValue = 0.f;
        }
    }
}

void CQuestionBlock::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
    if(mKickedValue != 0.f) renderStates.transform.translate(0.f, mKickedValue);
    CBlock::draw(renderTarget, renderStates);
}

void CQuestionBlock::kicked(CPlayer* player)
{
    if(!mIsSpawned)
    {
        mIsSpawned = true;
        CMarioWorld* world = static_cast<CMarioWorld*>(getParent()->getStage());
        setTile(world->getSpawnedTile());
        if(mSpawnItem == "Coin")
        {
            world->addPickUp(CCoin::createOnBlock(getPosition()));
        }
        else if(mSpawnItem == "PowerUp")
        {
            if(player->isSmall())
            {
                world->addPickUp(CMushroom::create(getPosition()));
            }
            else
            {
                world->addPickUp(CFlower::create(getPosition()));
            }
        }
        mKickedSpeed = -32.f * 2.f;
        kickEnemyAbove();
    }
    else if(mKickedSpeed == 0.f)
    {
        mKickedSpeed = -32.f * 2.f;
        kickEnemyAbove();
        SoundManager::play(EBump);
    }
}

void CQuestionBlock::setSpawnItemType(const std::string &spawnItem)
{
    mSpawnItem = spawnItem;
}

const std::string &CQuestionBlock::getSpawnItemType() const
{
    return mSpawnItem;
}
