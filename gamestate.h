#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Window/Event.hpp>
//pre-defintion
#include "base.h"
class CBaseGame;

class CGameState : public CObject
{
public:
    CGameState(const CGameState&) = delete ;
    CGameState& operator=(const CGameState&) = delete;


    CGameState(CBaseGame* game);
    virtual ~CGameState();

    virtual void update(TFloat deltaTime) = 0;
    virtual void draw(sf::RenderTarget& renderTarget) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    CBaseGame* getGame() const;



    virtual void drawDebug(sf::RenderTarget&){ }


private:
    CBaseGame* mGame;
};

#endif // GAMESTATE_H
