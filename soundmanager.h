#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>
#include "base.h"
enum TSoundID
{
    EOneUp,
    EBowserFire,
    EBowserFall,
    EBreakBlock,
    EBump,
    ECoin,
    EFireBall,
    EFlagPole,
    EGameOver,
    EJumpSuper,
    EMarioDie,
    EPipe,
    EPowerUp,
    EPowerUpAppear,
    EStageClear,
    EStomp,
    EWarning,
    EWorldClear,
    EKick
};
class SoundManager
{
public:
    static TBoolean load(const std::string& file, TInt id);

    static void unLoad(TInt id);

    static void play(TInt id);

    static void unLoadAll();
private:
    SoundManager();

    static std::map<TInt, sf::SoundBuffer*> soundBuffers;
    static sf::Sound soundPlayers[];
    static const TInt KSoundPlayerSize;
};

#endif // SOUNDMANAGER_H
