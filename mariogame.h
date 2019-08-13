#ifndef MARIOGAME_H
#define MARIOGAME_H
#include "basegame.h"
#define GAME_WIDTH 512
#define GAME_HEIGHT 864


class CMarioGame : public CBaseGame
{
public:
    CMarioGame();

    void onCreate() override;

    void onExit() override;
};

#endif // MARIOGAME_H
