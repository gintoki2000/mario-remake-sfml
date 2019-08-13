#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include "gamestate.h"
#include <stack>
#include <functional>
#include <list>
/*
 * hold and manage states in game
 *
*/
class CStateManager
{
public:
    CStateManager(const CStateManager&) = delete;
    CStateManager& operator=(const CStateManager&) = delete;
    CStateManager();
    ~CStateManager();

    CGameState* getCurrentState();

    void clear();

    void changeState(CGameState* state);

    void pushState(CGameState* state);

    void popState();

    TBoolean isEmpty() const;

    void applyPendingChanges();
private:
    std::list<CGameState*> mStateStack;//<<store states
    std::list<std::function<void()>> mActions;//delayed actions
};



#endif // STATEMANAGER_H
