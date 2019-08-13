#include "soundmanager.h"
std::map<TInt, sf::SoundBuffer*> SoundManager::soundBuffers = std::map<TInt, sf::SoundBuffer*>();
sf::Sound SoundManager::soundPlayers[10];
const TInt SoundManager::KSoundPlayerSize = 10;
TBoolean SoundManager::load(const std::string &file, TInt id)
{
    auto found = soundBuffers.find(id);
    if(found != soundBuffers.end()) return false;
    sf::SoundBuffer* soundBuffer = new (std::nothrow) sf::SoundBuffer();
    if(soundBuffer && soundBuffer->loadFromFile(file))
    {
        soundBuffers.insert(std::make_pair(id, soundBuffer));
        return true;
    }
    delete soundBuffer;
    return false;
}

void SoundManager::unLoad(TInt id)
{
    auto found = soundBuffers.find(id);
    if(found != soundBuffers.end())
    {
        delete found->second;
        soundBuffers.erase(found);
    }
}

void SoundManager::play(TInt id)
{
    auto found = soundBuffers.find(id);
    if(found == soundBuffers.end()) return;
    for(TInt i = 0; i < KSoundPlayerSize; i++)
    {
        if(soundPlayers[i].getStatus() == sf::Sound::Status::Stopped)
        {

            soundPlayers[i].setBuffer(*(found->second));
            soundPlayers[i].play();
            break;
        }
    }
}

void SoundManager::unLoadAll()
{
    for(const auto& pair : soundBuffers)
    {
        delete pair.second;
    }
    soundBuffers.clear();
}

SoundManager::SoundManager()
{

}
