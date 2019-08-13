#ifndef BASEGAME_H
#define BASEGAME_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "framerate.h"
#include "base.h"
#include "property.h"
class CStateManager;
class CBaseGame : public CObject
{
public:
    CBaseGame(const CBaseGame&) = delete;
    CBaseGame& operator=(const CBaseGame&) = delete;
    CBaseGame();
    virtual ~CBaseGame();


    //start game loop
    void start();

    //stop game loop
    void stop();

    //<set and get methods>//
    sf::Window& getWindow();

    CStateManager *getStateManager() const;

    TInt getFrameRate() const;

    TBoolean isDebugEnabled() const;

    void enableDebug();

    void disableDebug();

    CProperties* getSession();

protected:
    virtual void onCreate() = 0;
    virtual void onExit() = 0;
private:

    //<data>//
    sf::RenderWindow mWindow;
    sf::Event mEvent;
    CStateManager* mStateManager;
    FrameRate mFrameRate;
    TBoolean mIsStarted;
    TBoolean mIsDebugEnabled;
    CProperties* mSession;
};

#endif // BASEGAME_H
