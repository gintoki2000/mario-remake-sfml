#include "mainstate.h"
#include "mariogame.h"
#include "marioworld.h"
#include "utils.h"
#include <iostream>
CMainState::CMainState(CMarioGame *marioGame):
    CGameState (marioGame),
    mMarioWorld(nullptr),
    mIsNeedToLoadNewMarioWorld(false)
{
}

CMainState::~CMainState()
{
    if(mMarioWorld)
    {
        delete mMarioWorld;
        mMarioWorld = nullptr;
    }
}

void CMainState::update(TFloat deltaTime)
{

    if(mIsNeedToLoadNewMarioWorld)
    {
        if(mMarioWorld)
        {delete mMarioWorld; mMarioWorld = nullptr;}
        mMarioWorld = CMarioWorld::create(this, getGame()->getSession(), mMarioWorldToBeLoaded, mPortalPlayerWillSpawn);
        mIsNeedToLoadNewMarioWorld = false;
        mMarioWorldToBeLoaded = "";
        mPortalPlayerWillSpawn = "";

    }
    if(!mMarioWorld) return;

    mMarioWorld->update(deltaTime);
}

void CMainState::draw(sf::RenderTarget &renderTarget)
{
    if(!mMarioWorld) return;
    renderTarget.clear();
    mMarioWorld->draw(renderTarget);
}

void CMainState::drawDebug(sf::RenderTarget &renderTarget)
{
    if(mMarioWorld) mMarioWorld->drawDebug(renderTarget);
}

void CMainState::handleEvent(const sf::Event &event)
{

    if(event.type == sf::Event::Closed){ getGame()->stop();}
}

void CMainState::LoadNewWorld(const std::string &tmxFile, const std::string &spawnPortal)
{
    //delay loading
    mIsNeedToLoadNewMarioWorld = true;
    mMarioWorldToBeLoaded = tmxFile;
    mPortalPlayerWillSpawn = spawnPortal;
}
