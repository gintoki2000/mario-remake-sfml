#include "brickblock.h"
#include "player.h"
#include "soundmanager.h"
#include "tile.h"
#include "property.h"
#include "breakblockparticle.h"
#include "tiledmap.h"
#include "stage.h"
CBrickBlock::CBrickBlock()
{

}

TBoolean CBrickBlock::init(const sf::Vector2f &position, CTile *tile)
{
    if(!CBlock::init(position, tile)) return false;
    setRigid(true);
    if(!tile)
    {
        std::cout<<"tile can not be null(CBrickBlock::init)!"<<std::endl;
        return false;
    }
    auto spawnItemProperty = tile->getProperties()->get<CString>("SpawnItemType");
    if(!spawnItemProperty)
    {
        std::cout<<"there is no SpawnItem property(CBrickBlock::init)!"<<std::endl;
        return false;
    }
    mSpawnItem = spawnItemProperty->value();
    mKickedSpeed = 0.f;
    mKickedValue = 0.f;
    return true;
}

CBrickBlock *CBrickBlock::create(const sf::Vector2f &position, CTile *tile)
{
    CBrickBlock* self = new (std::nothrow) CBrickBlock();
    if(self && self->init(position, tile))
    {
        return self;
    }
    delete self;
    return nullptr;
}

void CBrickBlock::update(TFloat deltaTime)
{
    if(mKickedSpeed != 0.f)
    {
        mKickedSpeed += -mKickedValue * 32.f * deltaTime;
        mKickedValue += mKickedSpeed * deltaTime;

        if(mKickedValue > 0.f)
        {
            mKickedSpeed = 0.f;
            mKickedValue = 0.f;
        }
    }
}

void CBrickBlock::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
    if(mKickedValue != 0.f) renderStates.transform.translate(0.f , mKickedValue);
    CBlock::draw(renderTarget, renderStates);
}

void CBrickBlock::kicked(CPlayer* player)
{
    if(player->isSmall())
    {
        mKickedSpeed = -32.f * 2.f;
        kickEnemyAbove();
        SoundManager::play(TSoundID::EBump);
    }
    else if(mKickedValue == 0.f)
    {

        SoundManager::play(TSoundID::EBreakBlock);
        auto root = getParent()->getParent()->getStage()->getRoot();
        auto pos = getPosition() + getParent()->getBlockSize()/2.f;
        auto p1 = CBreakBlockParticle::create(pos, (KVecLeft * 0.5f + KVecUp * 2.f) * KBlockSize * 4.f);
        auto p2 = CBreakBlockParticle::create(pos, (KVecRight * 0.5f + KVecUp * 2.f) * KBlockSize * 4.f);
        auto p3 = CBreakBlockParticle::create(pos, (KVecLeft * 0.5f + KVecUp) * KBlockSize * 4.f);
        auto p4 = CBreakBlockParticle::create(pos, (KVecRight * 0.5f + KVecUp) * KBlockSize * 4.f);
        if(p1) root->attachChild(p1);
        root->attachChild(p2);
        root->attachChild(p3);
        root->attachChild(p4);
        kickEnemyAbove();
        destroy();
    }
}


