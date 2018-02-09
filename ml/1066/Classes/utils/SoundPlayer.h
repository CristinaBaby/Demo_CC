//
//  SoundPlayer.h
//  Doctor
//
//  Created by tanshoumei on 13-8-23.
//
//

#ifndef __Doctor__SoundPlayer__
#define __Doctor__SoundPlayer__

#include <iostream>
#include "SingleTon.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

class SoundPlayer: public SingleTon<SoundPlayer>
{
    friend class SingleTon<SoundPlayer>;
private:
    std::string bgMusicName;
    SoundPlayer();
    float soundV,musicV;
public:
    //背景音乐
    void playBackGroundMusic(const std::string &audioName);
    void pauseBackGroundMusic();
    void resumeBackGroundMusic();
    void stopBackGroundMusic();
    void stopAllEffect();
    void playEffect(const std::string &audioName);
    int  playEffectLoop(const std::string &audioName);
    void stopEffectLoop(const int soundId);
    void initVolume();
    void flushVolume();
    void setSoundV(float f);
    void setMusicV(float f);
    float getSoundV();
    float getMusicV();
};
#define SoundPlayerInstance SoundPlayer::getInstance()
#endif /* defined(__Doctor__SoundPlayer__) */
