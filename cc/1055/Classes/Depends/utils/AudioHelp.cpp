//
//  AudioHelp.cpp
//  MACY013
//
//  Created by wuguiling on 6/27/14.
//
//

#include "AudioHelp.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

#define kIsLauched "Lanched"
#define kEffect  "effect"
#define kMusic "music"
#define kEffectVolume  "effectVolume"
#define kMusicVolume "musicVolume"

AudioHelp::AudioHelp():m_nLoopId(0),m_bVolumeMode(false)
{
    if (!UserDefault::getInstance()->getBoolForKey(kIsLauched)) {
        UserDefault::getInstance()->setBoolForKey(kEffect, true);
        UserDefault::getInstance()->setBoolForKey(kMusic, true);
        
        UserDefault::getInstance()->setFloatForKey(kEffectVolume, 1.0f);
        UserDefault::getInstance()->setFloatForKey(kMusicVolume, 1.0f);
        
       UserDefault::getInstance()->setBoolForKey(kIsLauched, true);
    }
    m_bEffectOpen = UserDefault::getInstance()->getBoolForKey(kEffect);
    m_bMusicOpen = UserDefault::getInstance()->getBoolForKey(kMusic);
    m_fEffectVolume = UserDefault::getInstance()->getFloatForKey(kEffectVolume);
    m_fMusicVolume = UserDefault::getInstance()->getFloatForKey(kMusicVolume);
}

AudioHelp::~AudioHelp()
{
    
}

const char* AudioHelp::_getSoundPath(const char *name)
{

    return  __String::createWithFormat("Sounds/%s",name)->getCString();
}

void AudioHelp::switchEffect()
{
    m_bEffectOpen = !m_bEffectOpen;
    SimpleAudioEngine::getInstance()->stopAllEffects();
    stopLoofEffect();
    m_nLoopId = 0;
    
//    if (!m_bEffectOpen) {
//        pauseLoofEffect();
//    }else{
//        resumeLoofEffect();
//    }
    UserDefault::getInstance()->setBoolForKey(kEffect, m_bEffectOpen);
    UserDefault::getInstance()->flush();
}

void AudioHelp::switchSound()
{
    m_bMusicOpen = !m_bMusicOpen;
    if (m_bMusicOpen) {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }else{
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    UserDefault::getInstance()->setBoolForKey(kMusic, m_bMusicOpen);
    UserDefault::getInstance()->flush();
}

void AudioHelp::setEffectVolume(float volume)
{
    
    m_fEffectVolume = volume;
    
    SimpleAudioEngine::getInstance()->setEffectsVolume(m_fEffectVolume);
    UserDefault::getInstance()->setFloatForKey(kEffectVolume, m_fEffectVolume);
    UserDefault::getInstance()->flush();
}

void AudioHelp::setSoundVolume(float volume)
{
    m_fMusicVolume = volume;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_fMusicVolume);
    UserDefault::getInstance()->setFloatForKey(kMusicVolume, m_fMusicVolume);
    UserDefault::getInstance()->flush();
}

float AudioHelp::getSoundVolume()
{
    
    return m_fMusicVolume;
}

float AudioHelp::getEffectVolume()
{
    return m_fEffectVolume;
}

void AudioHelp::playBackgroundMusic(const char *name){
    SimpleAudioEngine::getInstance()->playBackgroundMusic(_getSoundPath(name),true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_fMusicVolume);
    if (!m_bMusicOpen) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void AudioHelp::StopBackgroundMusic()
{
    
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioHelp::PauseBackgroundMusic()
{
    
    if (m_bMusicOpen) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void AudioHelp::ResumeBackgroundMusic()
{
    if (m_bMusicOpen) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_fMusicVolume);
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    
}

void AudioHelp::playEffect(const char *name)
{
    if (!m_bEffectOpen&&!m_bVolumeMode)
        return;
    SimpleAudioEngine::getInstance()->setEffectsVolume(m_fEffectVolume);
    SimpleAudioEngine::getInstance()->playEffect(_getSoundPath(name));
}

void AudioHelp::playLoofEffectOnce(const char *name)
{
    if ((!m_bEffectOpen||m_nLoopId!=0)&&!m_bVolumeMode)
        return;
    SimpleAudioEngine::getInstance()->setEffectsVolume(m_fEffectVolume);
    m_nLoopId = SimpleAudioEngine::getInstance()->playEffect(_getSoundPath(name), true);
}

void AudioHelp::playLoofEffect(const char *name)
{
    if ((!m_bEffectOpen||m_nLoopId!=0)&&!m_bVolumeMode){
        CCLOG("------playLoofEffect----return");
        return;
    }
    m_nLoopId = SimpleAudioEngine::getInstance()->playEffect(_getSoundPath(name), true);
    
}
void AudioHelp::stopLoofEffect()
{
    if (m_nLoopId == 0) {
        return;
    }
    SimpleAudioEngine::getInstance()->stopEffect(m_nLoopId);
    m_nLoopId = 0;
}

void AudioHelp::pauseLoofEffect()
{
    SimpleAudioEngine::getInstance()->pauseEffect(m_nLoopId);
}

void AudioHelp::resumeLoofEffect()
{
    SimpleAudioEngine::getInstance()->resumeEffect(m_nLoopId);
}
void AudioHelp::playStartEffect()
{
    playEffect("play.mp3");
}

void AudioHelp::playNextEffect()
{
    playEffect("next page.mp3");
}

void AudioHelp::playPreEffect()
{
    playEffect("previous page.mp3");
}
void AudioHelp::playResetEffect()
{
    playEffect("reset.mp3");
}

void AudioHelp::playSelectEffect()
{
    playEffect("all button_sound.mp3");
}

void AudioHelp::playCameraEffect()
{
    playEffect("button_camera.mp3");
}

void AudioHelp::playDeleteEffect()
{
    playEffect("delete.mp3");
}

static int gsWaterId = 0;
void AudioHelp::playWaterLoopEffect()
{
    if (gsWaterId) {
        return;
    }
    gsWaterId = SimpleAudioEngine::getInstance()->playEffect(_getSoundPath("add water.mp3"), true);
}

void AudioHelp::stopWaterEffect()
{
    SimpleAudioEngine::getInstance()->stopEffect(gsWaterId);
    gsWaterId = 0;
}

void AudioHelp::playBrushLoopEffect()
{
    playLoofEffect("brush.mp3");
}

void AudioHelp::playBigBrushLoopEffect()
{
    playLoofEffect("big brush.mp3");
}

void AudioHelp::playSpongeLoopEffect()
{
    playLoofEffect("sponge.mp3");
}

void AudioHelp::playTowelLoopEffect()
{
    playLoofEffect("towel.mp3");
}

void AudioHelp::playShowerLoopEffect()
{
    playLoofEffect("shower.mp3");
}

void AudioHelp::playCarRunLoopEffect()
{
    playLoofEffect("car run.mp3");
}

void AudioHelp::playCraneLoopEffect()
{
    playLoofEffect("animation_crane.mp3");
}

void AudioHelp::playWaxEffect()
{
    playEffect("wax.mp3");
}

void AudioHelp::playJumpEffect()
{
    playEffect("bounce.mp3");
}
void AudioHelp::playPumpEffect()
{
    playEffect("pump.mp3");
}
void AudioHelp::playHonkEffect()
{
    playEffect("honk.mp3");
}

void AudioHelp::playChooseTruckEffect()
{
    playEffect("choose a truck.mp3");
}
void AudioHelp::playCongratulationEffect()
{
    playEffect("congratulation.mp3");
}

void AudioHelp::playRightEffect()
{
    playEffect("right.mp3");
}

void AudioHelp::playWrongEffect()
{
    playEffect("wrong.mp3");
}
