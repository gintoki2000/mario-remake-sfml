#include "mariogame.h"
#include "mainstate.h"
#include "statemanager.h"
#include "texturemanager.h"
#include <SFML/Graphics/Texture.hpp>
#include "utils.h"
#include "soundmanager.h"
#include "player.h"
#include "fontmanager.h"
CMarioGame::CMarioGame()
{

}

void CMarioGame::onCreate()
{
    //setup window
    LOG("Seting up window..");
    getWindow().create(sf::VideoMode(864, 512), "Super Mario");
    getWindow().setFramerateLimit(60);

    //load all textures
    LOG("Load all resources..");
    const std::string texturePath("resources/textures");
    TextureManager::load(texturePath + "/Mario.png", TTextureID::EMarioTextureID);
    TextureManager::load(texturePath + "/Items.png", TTextureID::EItemsTextureID);
    TextureManager::load(texturePath + "/Enemies.png", TTextureID::EEnemiesTextureID);


    //load all sound
    std::string soundPath("resources/sounds");
    SoundManager::load(soundPath + "/1-up.wav", TSoundID::EOneUp);
    SoundManager::load(soundPath + "/bowser_falls.wav", TSoundID::EBowserFall);
    SoundManager::load(soundPath + "/bowser_fire.wav", TSoundID::EBowserFire);
    SoundManager::load(soundPath + "/breakblock.wav", TSoundID::EBreakBlock);
    SoundManager::load(soundPath + "/bump.wav", TSoundID::EBump);
    SoundManager::load(soundPath + "/coin.wav", TSoundID::ECoin);
    SoundManager::load(soundPath + "/fireball.wav", TSoundID::EFireBall);
    SoundManager::load(soundPath + "/flagpole.wav", TSoundID::EFlagPole);
    SoundManager::load(soundPath + "/game_over.wav", TSoundID::EGameOver);
    SoundManager::load(soundPath + "/jump_super.wav", TSoundID::EJumpSuper);
    SoundManager::load(soundPath + "/kick.wav", TSoundID::EKick);
    SoundManager::load(soundPath + "/mario_die.wav", TSoundID::EMarioDie);
    SoundManager::load(soundPath + "/pipe.wav", TSoundID::EPipe);
    SoundManager::load(soundPath + "/powerup.wav", TSoundID::EPowerUp);
    SoundManager::load(soundPath + "/powerup_appears.wav", TSoundID::EPowerUpAppear);
    SoundManager::load(soundPath + "/stage_clear.wav", TSoundID::EStageClear);
    SoundManager::load(soundPath + "/stomp.wav", TSoundID::EStomp);
    SoundManager::load(soundPath + "/warning.wav", TSoundID::EWarning);
    SoundManager::load(soundPath + "/world_clear.wav", TSoundID::EWorldClear);

    FontManager::load("resources/fonts/Ubuntu-Regular.ttf", EUbuntuRegularFont);

    getSession()->set("powerUpState", new CProperty<CPlayer::TPowerUpState>(CPlayer::TPowerUpState::ENormal));

    enableDebug();
    //setup fist state
    LOG("Setting up first state...");
    CMainState* mainState = new CMainState(this);
    mainState->LoadNewWorld("resources/worlds/world-1-1.tmx");
    getStateManager()->pushState(mainState);

    LOG("Game started!");
}

void CMarioGame::onExit()
{
    LOG("Clean up...");
    TextureManager::unloadAll();
    SoundManager::unLoadAll();
    FontManager::unLoadAll();
    LOG("Game exit!");
}
