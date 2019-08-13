#ifndef MARIOWORLD_H
#define MARIOWORLD_H
#include "stage.h"
#include <string>
#include "camera.h"
#include "tiledmap.h"
#include "tilesets.h"
class CMainState;
class CPlayer;
class CEnemy;
class CItem;
class CBlock;
class CPortal;
class CParticle;
class CSession;
class CPickUp;

/*
 *                                  root
 *                                    *
 *                      *************************************
 *                      *       *       *       *           *
 *                    player  enemies  items    tiledmap   HUD
*/
class CMarioWorld : public CStage
{
public:
    CMarioWorld(const CMarioWorld&) = delete ;

    ~CMarioWorld();

    static CMarioWorld* create(CMainState* mainGameState, CProperties* session, const std::string& file,const std::string& spawnPortal);



    CPlayer* getPlayer() const;

    CNode* getEnemies() const;

    CNode* getItems() const;

    CTiledMap* getTiledMap() const;

    void addEnemy(CEnemy* enemy);

    void addItem(CItem* item);

    void addParticle(CParticle* particle);

    void addPickUp(CPickUp* pickup);

    CTileSets *getTilesets() const;

    void activateScene();

    void deactiveScene();

    CMainState *getState() const;

    CNode *getPickUps() const;

    CNode *getPortals() const;

    CTile *getSpawnedTile() const;


    void update(TFloat deltaTime) override;
private:

    CMarioWorld();

    TBoolean init(CMainState* state, CProperties* session, const std::string& world, const std::string& spawnPortal);



    void updateCamera(TFloat deltaTime);

    CMainState* mState;

    CNode* mEnemies;//<<store node which contains all enemies

    CNode* mItems;//<<store node which contains all items

    CNode* mPortals;//<<store node which contains all portals

    CNode* mPickUps;

    CPlayer* mPlayer;//<<store player object

    CTiledMap* mTiledLayer;//<<store tiled layer object

    CTileSets* mTilesets;

    CTile* mSpawnedTile;

    CMainState* mMainState;

    CProperties* mSession;

    CProperties* mWorldProperties;

};
#endif // MARIOWORLD_H
