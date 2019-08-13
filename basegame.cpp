#include "basegame.h"
#include "gamestate.h"
#include "statemanager.h"
#include <iostream>
CBaseGame::CBaseGame():
    mStateManager(new CStateManager()),
    mIsStarted(false),
    mIsDebugEnabled(false),
    mSession(new CProperties())
{

}

CBaseGame::~CBaseGame()
{
    delete mStateManager;
    delete mSession;
}

void CBaseGame::start()
{
    using namespace std;
    if(!mIsStarted)
    {

        onCreate();
        getStateManager()->applyPendingChanges();
        mIsStarted = true;
        sf::Clock clock;
        sf::Event event;
        try{
            while (!(getStateManager()->isEmpty())) {
                CGameState* state = getStateManager()->getCurrentState();
                TFloat deltaTime = clock.restart().asSeconds();
                while (getWindow().pollEvent(event)) {
                    state->handleEvent(event);
                }
                mFrameRate.tick(deltaTime);
                state->update(1.f / 60.f);
                state->draw(mWindow);
                if(isDebugEnabled()) state->drawDebug(mWindow);
                mWindow.display();
                getStateManager()->applyPendingChanges();
            }
        }catch(const std::exception& e)
        {
            std::cout<<typeid(e).name()<<": "<<e.what()<<std::endl;
        }
        onExit();
    }
}

void CBaseGame::stop()
{
    mStateManager->clear();

}

sf::Window &CBaseGame::getWindow()
{
    return mWindow;
}

CStateManager *CBaseGame::getStateManager() const
{
    return mStateManager;
}

TInt CBaseGame::getFrameRate() const
{
    return mFrameRate.getCurrentFrameRate();
}

TBoolean CBaseGame::isDebugEnabled() const
{
    return mIsDebugEnabled;
}

void CBaseGame::enableDebug()
{
    mIsDebugEnabled = true;
}

void CBaseGame::disableDebug()
{
    mIsDebugEnabled = false;
}

CProperties *CBaseGame::getSession()
{
    return mSession;
}

