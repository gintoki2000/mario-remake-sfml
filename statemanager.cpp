#include "statemanager.h"

CStateManager::CStateManager()
{

}

CStateManager::~CStateManager()
{
    for(const auto& state : mStateStack) delete state;
    mStateStack.clear();
    mActions.clear();
}

CGameState *CStateManager::getCurrentState()
{
    if(isEmpty()) return nullptr;
    return mStateStack.back();
}

void CStateManager::clear()
{
    auto clearAction = [=](){
        for(const auto& state : mStateStack) delete state;
        mStateStack.clear();
    };

    mActions.push_back(clearAction);
}

void CStateManager::changeState(CGameState *state)
{
    auto changeAction = [=]{
        if(!isEmpty())
        {
            CGameState* topState = mStateStack.back();
            delete topState;
            mStateStack.pop_back();
        }
        mStateStack.push_back(state);
    };
    mActions.push_back(changeAction);


}

void CStateManager::pushState(CGameState *state)
{
    auto pushAction = [=]{
        mStateStack.push_back(state);
    };
    mActions.push_back(pushAction);
}

void CStateManager::popState()
{
    auto popAction = [=]{
        if(!isEmpty())
        {
            CGameState* topState = mStateStack.back();
            delete topState;
            mStateStack.pop_back();
        }
    };
    mActions.push_back(popAction);
}




TBoolean CStateManager::isEmpty() const
{
    return mStateStack.empty();
}

void CStateManager::applyPendingChanges()
{
    for(const auto& action : mActions) action();
    mActions.clear();
}
