#ifndef MAINSTATE_H
#define MAINSTATE_H
#include "gamestate.h"
class CMarioGame;
class CMarioWorld;

class CMainState : public CGameState
{
public:
    CMainState(CMarioGame* marioGame);

    ~CMainState();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget) override;

    void drawDebug(sf::RenderTarget& renderTarget) override;

    void handleEvent(const sf::Event& event) override;

    //
    void LoadNewWorld(const std::string& tmxFile, const std::string& spawnPortal = "");


private:
    CMarioWorld* mMarioWorld;
    TBoolean mIsNeedToLoadNewMarioWorld;
    std::string mMarioWorldToBeLoaded;
    std::string mPortalPlayerWillSpawn;
};

#endif // MAINSTATE_H
