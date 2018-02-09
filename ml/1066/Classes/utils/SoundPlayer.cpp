//
//  SoundPlayer.cpp
//  Doctor
//
//  Created by tanshoumei on 13-8-23.
//
//

#include "SoundPlayer.h"
#include "cocos2d.h"

using namespace cocos2d;
#define sound_key "game_sound"
#define music_key "game_music"

SoundPlayer::SoundPlayer()
{
    bgMusicName = " ";
    initVolume();
}

void SoundPlayer::playBackGroundMusic(const std::string &audioName)
{
    if(audioName == bgMusicName)
        return;
    bgMusicName = audioName;
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(musicV);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(audioName.c_str(), true);
}

void SoundPlayer::pauseBackGroundMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundPlayer::resumeBackGroundMusic()
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void SoundPlayer::stopBackGroundMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundPlayer::stopAllEffect()
{    
  
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void SoundPlayer::playEffect(const std::string &audioName)
{
  
    SimpleAudioEngine::getInstance()->setEffectsVolume(soundV);
    SimpleAudioEngine::getInstance()->playEffect(audioName.c_str());
}

int SoundPlayer::playEffectLoop(const std::string &audioName)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(soundV);
    return SimpleAudioEngine::getInstance()->playEffect(audioName.c_str(), true);
}

void SoundPlayer::stopEffectLoop(const int soundId)
{
    SimpleAudioEngine::getInstance()->stopEffect(soundId);
}

void SoundPlayer::flushVolume() {
    UserDefault::getInstance()->setFloatForKey(sound_key, soundV);
    UserDefault::getInstance()->setFloatForKey(music_key, musicV);
    UserDefault::getInstance()->flush();
}

void SoundPlayer::initVolume(){
    soundV = UserDefault::getInstance()->getFloatForKey(sound_key,1.0f);
    musicV = UserDefault::getInstance()->getFloatForKey(music_key,1.0f);
}

void SoundPlayer::setSoundV(float f) {
    soundV = f;
}
void SoundPlayer::setMusicV(float f) {
    musicV = f;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(f);
}

float SoundPlayer::getSoundV() {
    return soundV;
}

float SoundPlayer::getMusicV() {
    return musicV;
}
