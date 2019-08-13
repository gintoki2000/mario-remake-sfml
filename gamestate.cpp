#include "gamestate.h"

CGameState::CGameState(CBaseGame* game):
    mGame(game)
{

}

CGameState::~CGameState()
{

}

CBaseGame *CGameState::getGame() const
{
    return mGame;
}


