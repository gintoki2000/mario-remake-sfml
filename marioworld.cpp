#include "marioworld.h"
#include "mariogame.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/Tileset.hpp"
#include "mainstate.h"
#include <iostream>
#include <map>
#include <functional>
#include "particle.h"

//<blocks>
#include "questionblock.h"
#include "brickblock.h"
#include "staticblock.h"

//<tilelayer>
#include "tileset.h"
#include "tiledmap.h"
#include "animatedtile.h"
#include "statictile.h"

#include "player.h"

//enemies
#include "goomba.h"
#include "koopa.h"
#include "piranhaplant.h"



//pickups
#include "coin.h"

//items
#include "onewayplatform.h"
#include "jumper.h"

#include "portal.h"

//------------------------//
#include "objectgroup.h"
#include "property.h"
CObject* createProperty(const tmx::Property& property)
{

    switch (property.getType()) {
    case tmx::Property::Type::Int:
        return new (std::nothrow) CInt(property.getIntValue());
    case tmx::Property::Type::Float:
        return new (std::nothrow) CFloat(property.getFloatValue());
    case tmx::Property::Type::Boolean:
        return new (std::nothrow) CBoolean(property.getBoolValue());
    case tmx::Property::Type::String:
        return new (std::nothrow) CString(property.getStringValue());
    default:
        return nullptr;
    }
}

CTileset* parseTileset(CTileset* tileset, const tmx::Tileset& tilesetData)
{
    sf::Texture* resource = new sf::Texture();
    if(resource->loadFromFile(tilesetData.getImagePath()))
    {
        tileset->setName(tilesetData.getName());
        tileset->setResource(resource);
        //create textureregions
        std::vector<TextureRegion*> textureRegions;
        for(const auto& tileData : tilesetData.getTiles())
        {
            TextureRegion* textureRegion = new TextureRegion();
            textureRegion->setTexture(*resource);
            textureRegion->setTextureRect(sf::IntRect(tileData.imagePosition.x, tileData.imagePosition.y, tileData.imageSize.x, tileData.imageSize.y));
            textureRegions.push_back(textureRegion);
        }
        tileset->setTextureRegions(textureRegions);
        //parse
        for(const auto& tileData : tilesetData.getTiles())
        {
            if(tileData.animation.frames.size()) //animated tile
            {
                std::vector<TUInt> intervals;
                std::vector<TextureRegion*> frames;
                for(const auto& frameData : tileData.animation.frames)
                {
                    intervals.push_back(frameData.duration);
                    frames.push_back(textureRegions[frameData.tileID - 1]);
                }
                CTile* tile = new CAnimatedTile(frames, intervals);
                tile->setType(tileData.type);
                for(const auto& property : tileData.properties)
                    tile->getProperties()->put(property.getName(), createProperty(property));
                tileset->putTile(tileData.ID + tilesetData.getFirstGID(), tile);
            }else
            {
                CTile* tile = new CStaticTile(textureRegions[tileData.ID]);
                tile->setType(tileData.type);
                for(const auto& property : tileData.properties)
                    tile->getProperties()->put(property.getName(), createProperty(property));
                tileset->putTile(tileData.ID + tilesetData.getFirstGID(), tile);
            }
        }
        std::cout<<"[LOG]: Created tileset \'"<<tileset->getName()<<'\''<<std::endl;
        return tileset;
    }else {
        delete resource;
        std::cout<<"[ERROR]: Could not create tileset \'"<<tilesetData.getName()<<'\''<<std::endl;
        return nullptr;
    }
}

template<class __block_type>
__block_type* createBlock(const sf::Vector2f& position, CTile* tile)
{
    return __block_type::create(position, tile);
}

void parseTilelayer(const tmx::TileLayer& tilelayerData, CTileSets* tilesets, CTiledMap* tiledlayer)
{
    std::map<std::string, std::function<CBlock*(const sf::Vector2f&, CTile*)>> factory =
    {
        {"QuestionBlock", createBlock<CQuestionBlock>},
        {"BrickBlock", createBlock<CBrickBlock>},
        {"StaticBlock", createBlock<CStaticBlock>}
    };

    tiledlayer->setName(tilelayerData.getName());
    tiledlayer->setVisible(tilelayerData.getVisible());
    tiledlayer->setPosition(tilelayerData.getOffset().x, tilelayerData.getOffset().y);
    const sf::Vector2f blockSize = tiledlayer->getBlockSize();
    for(TInt x = 0; x < tiledlayer->getWidth(); x++)
    {
        for(TInt y = 0; y < tiledlayer->getHeight(); y++)
        {
            TUInt tileID = tilelayerData.getTiles()[x + y*(tiledlayer->getWidth())].ID;
            CTile* tile = tilesets->getTile(tileID);
            if(tile)
            {
                auto found = factory.find(tile->getType());
                if(found != factory.end())
                {
                    sf::Vector2f blockPosition(blockSize.x * x, blockSize.y * y);
                    CBlock* block = found->second(blockPosition, tile);
                    tiledlayer->setBlock(x, y, block);
                }
                else
                {
                    LOG_ERR("Could not find create function for \'"+tile->getType()+"\'");
                }
            }
        }
    }

}



void parseObjectGroup(const tmx::ObjectGroup& objectGroupData, CNode* objectGroup, CProperties* session)
{
    static std::map<std::string, std::function<CGameObject*(CProperties* objectProperties)>> factories =
    {
        {"Player",          CPlayer::create},
        {"Goomba",          CGoomba::create},
        {"Koopa",           CKoopa::create},
        {"Coin",            CCoin::create},
        {"PiranhaPlant",    CPiranhaPlant::create},
        {"OneWayPlatform",  COneWayPlatform::create},
        {"Jumper", CJumper::create},
        {"Portal", CPortal::create}
    };

    //---------------------------------------------------------------------------
    objectGroup->setName(objectGroupData.getName());
    objectGroup->setPosition(objectGroupData.getOffset().x, objectGroupData.getOffset().y);
    for(const auto& objectData : objectGroupData.getObjects())
    {
        const auto& found = factories.find(objectData.getType());
        if(found != factories.end())
        {
            CProperties objectProperties;
            objectProperties.put("x", new (std::nothrow) CFloat(objectData.getPosition().x));
            objectProperties.put("y", new (std::nothrow) CFloat(objectData.getPosition().y));
            objectProperties.put("width", new (std::nothrow) CFloat(objectData.getAABB().width));
            objectProperties.put("height", new (std::nothrow) CFloat(objectData.getAABB().height));
            objectProperties.put("name", new (std::nothrow) CString(objectData.getName()));
            objectProperties.put("session", new (std::nothrow) CProperty<CProperties*>(session));
            for(const auto& property : objectData.getProperties())
            {
                objectProperties.put(property.getName(), createProperty(property));
            }
            CGameObject* gameObject = found->second(&objectProperties);
            if(gameObject)
            {
                gameObject->setName(objectData.getName());
                objectGroup->attachChild(gameObject);
            }
        }
        else
        {
            std::cout<<"[LOG]: Could not find object type \'"<<objectData.getType()<<"\'."<<std::endl;
        }
    }//for
}

CMarioWorld::~CMarioWorld()
{
    delete mTilesets;
}
CMarioWorld *CMarioWorld::create(CMainState *state, CProperties *session, const std::string &file, const std::string &spawnPortal)
{
    CMarioWorld* self = new (std::nothrow) CMarioWorld();
    if(self && self->init(state, session, file, spawnPortal))
    {
        return self;
    }
    else
    {
        delete self;
        return nullptr;
    }
}



CPlayer *CMarioWorld::getPlayer() const
{
    return mPlayer;
}

CNode *CMarioWorld::getEnemies() const
{
    return mEnemies;
}

CNode *CMarioWorld::getItems() const
{
    return mItems;
}

CTiledMap *CMarioWorld::getTiledMap() const
{
    return mTiledLayer;
}

void CMarioWorld::addEnemy(CEnemy *enemy)
{
    getEnemies()->attachChild(enemy);
}

void CMarioWorld::addParticle(CParticle *particle)
{
    getRoot()->attachChild(particle);
}

void CMarioWorld::addPickUp(CPickUp *pickup)
{
    getPickUps()->attachChild(pickup);
}
CMarioWorld::CMarioWorld()
{

}

TBoolean CMarioWorld::init(CMainState *state, CProperties* session, const std::string &world, const std::string &spawningPortal)
{
    tmx::Map worldData;
    mState = state;
    if(worldData.load(world))
    {
        mTilesets = new (std::nothrow) CTileSets();
        if(!mTilesets) return false;
        //parse tilesets
        for(const auto& tilesetData : worldData.getTilesets())
        {
            CTileset* tileset = new (std::nothrow) CTileset();
            parseTileset(tileset, tilesetData);
            getTilesets()->addTileset(tileset);
        }
        mSpawnedTile = nullptr;
        for(const auto& tileset : getTilesets()->getItems())
        {
            for(const auto& pair : tileset->getItems())
            {
                if(pair.second->getType() == "Spawned")
                {
                    mSpawnedTile = pair.second;
                    break;
                }
            }
        }
        if(!mSpawnedTile) return false;
        for(const auto& layerData : worldData.getLayers())
        {
            if(layerData->getType() == tmx::Layer::Type::Tile)
            {
                CTiledMap* tiledlayer = CTiledMap::create(worldData.getTileCount().x, worldData.getTileCount().y, worldData.getTileSize().x, worldData.getTileSize().y);
                if(tiledlayer)
                {
                    getRoot()->attachChild(tiledlayer);
                    parseTilelayer(*static_cast<tmx::TileLayer*>(layerData.get()), getTilesets(), tiledlayer);
                }
            }
            else if(layerData->getType() == tmx::Layer::Type::Object)
            {
                CNode* objectGroup = new (std::nothrow) CObjectGroup();
                if(objectGroup)
                {
                    getRoot()->attachChild(objectGroup);
                    parseObjectGroup(*static_cast<tmx::ObjectGroup*>(layerData.get()), objectGroup, session);
                }
            }
        }

        //store game's components
        mPlayer = static_cast<CPlayer*>(getRoot()->findNodeByName("Player"));
        mTiledLayer = static_cast<CTiledMap*>(getRoot()->findNodeByName("TiledMap"));
        mEnemies = getRoot()->findNodeByName("Enemies");
        mPortals = getRoot()->findNodeByName("Portals");
        mItems =   getRoot()->findNodeByName("Items");
        mPickUps = getRoot()->findNodeByName("PickUps");

        if(!getPlayer())
        {
            LOG_ERR("there is no \"Player\" instance!");
            return false;
        }

        if(!getTiledMap())
        {
            LOG_ERR("there is no \"TiledLayer\" instance!");
            return false;
        }

        if(!mEnemies)
        {
            mEnemies = new (std::nothrow) CObjectGroup;
            if(mEnemies)
            {
                getRoot()->attachChild(mEnemies);
            }
            else{ LOG_ERR("could not allocate instance of CNode(Enemies)!"); return false;}
        }

        if(!getPortals())
        {
            LOG_ERR("there is no portals!");
            return false;
        }

        if(!getItems())
        {
            mItems = new (std::nothrow) CObjectGroup();
            if(mItems)
            {
                getRoot()->attachChild(getItems());

            }
            else {
                LOG_ERR("could not allocate  instance of CNode(Items)!");
                return false;
            }
        }

        if(!getPickUps())
        {
            mPickUps = new (std::nothrow) CObjectGroup();
            if(getPickUps())
            {
                getRoot()->attachChild(getPickUps());
            }
            else {
                LOG_ERR("colud not allocate instance of CNode(PickUps)");
                return false;
            }
        }


        mPortals->deactivate();




        //destroy player's container and attach player to root node
        auto pc = getPlayer()->getParent();
        pc->deattachChild(getPlayer());
        getRoot()->removeChild(pc);
        getRoot()->attachChild(getPlayer());

        getTiledMap()->toFront();

        getPlayer()->toBack();

        auto portal = getPortals()->findNodeByName(spawningPortal);
        if(portal) getPlayer()->exitPortal(static_cast<CPortal*>(portal));

        //setup camera
        sf::Vector2u windowSize = state->getGame()->getWindow().getSize();
        getCamera().setSize(windowSize.x, windowSize.y);
        getCamera().setCenter(windowSize.x / 2.f , windowSize.y / 2.f);
        getCamera().setViewport(sf::FloatRect(0.0f, 0.0f , 1.f, 1.f));
        return true;
    }
    else {
        return false;
    }
}

void CMarioWorld::update(TFloat deltaTime)
{
    CStage::update(deltaTime);
    updateCamera(deltaTime);
}

void CMarioWorld::updateCamera(TFloat deltaTime)
{
    getCamera().setCenter(getPlayer()->getPosition().x, getCamera().getCenter().y);
}



CTile *CMarioWorld::getSpawnedTile() const
{
    return mSpawnedTile;
}

CNode *CMarioWorld::getPortals() const
{
    return mPortals;
}

CNode *CMarioWorld::getPickUps() const
{
    return mPickUps;
}

CMainState *CMarioWorld::getState() const
{
    return mState;
}

CTileSets *CMarioWorld::getTilesets() const
{
    return mTilesets;
}

void CMarioWorld::activateScene()
{
    for(auto& object : getRoot()->getChildren())
    {
        if(!object->isTypeOf<CPlayer>())
        {
            object->activate();
        }
    }
    mPortals->deactivate();
}

void CMarioWorld::deactiveScene()
{
    for(auto& object : getRoot()->getChildren())
    {
        if(!object->isTypeOf<CPlayer>())
        {
            object->deactivate();
        }
    }
}
