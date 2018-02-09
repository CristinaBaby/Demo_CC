//
//  AudioHelper.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "SimpleAudioEngine.h"
#include "AudioHelper.h"
#include "../AppGlobal.h"
#include "../utilities/STUserDefault.h"
#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

USING_NS_ST;
using namespace CocosDenshion;

#define GetAudioEngine              SimpleAudioEngine::getInstance()

#define DEFAULT_VOLUME              0.8

static AudioHelper* s_m_AudioHelper = NULL;

AudioHelper* AudioHelper::getInstance()
{
    if (NULL == s_m_AudioHelper)
    {
        s_m_AudioHelper = new AudioHelper();
    }
    
    return s_m_AudioHelper;
}

void AudioHelper::destroy()
{
    CC_SAFE_DELETE(s_m_AudioHelper);
}

void AudioHelper::changeMusicVolume(float volume)
{
    if (mMusicVolume == volume)
        return;
    
    mMusicVolume = volume;
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    
    if (mMusicVolume <=0 && GetAudioEngine->isBackgroundMusicPlaying()) {
        GetAudioEngine->stopBackgroundMusic();
    }
    
    STSaveFloatToUD(key_music_volume, mMusicVolume);
}

void AudioHelper::changeSoundVolume(float volume)
{
    if (mSoundVolume == volume)
        return;
    
    mSoundVolume = volume;
    GetAudioEngine->setEffectsVolume(mSoundVolume);
    STSaveFloatToUD(key_sound_volume, mSoundVolume);
}

unsigned AudioHelper::playSound(const char* sound, bool bLoop /*= false*/)
{
#if TARGET_IPHONE_SIMULATOR == 0
    if (mSoundVolume <= 0)
        return 0;
    
    return GetAudioEngine->playEffect(sound, bLoop);
#else
    return 0;
#endif
}

unsigned AudioHelper::playDefaultSound()
{
#if TARGET_IPHONE_SIMULATOR == 0
    if (mSoundVolume <= 0)
        return 0;
    
    return GetAudioEngine->playEffect("sfx_btn_default.mp3", false);
#else
    return 0;
#endif
}

void AudioHelper:: preloadSound(const char* pszFilePath)
{
#if TARGET_IPHONE_SIMULATOR == 0
    GetAudioEngine->preloadEffect(pszFilePath);
#endif
}

void AudioHelper:: unloadSound(const char* pszFilePath)
{
#if TARGET_IPHONE_SIMULATOR == 0
    GetAudioEngine->unloadEffect(pszFilePath);
#endif
}

void AudioHelper::stopSound(unsigned int nSoundId)
{
    GetAudioEngine->stopEffect(nSoundId);
}

void AudioHelper::stopAllSounds()
{
    GetAudioEngine->stopAllEffects();
}

void AudioHelper:: pauseSound(unsigned int nSoundId)
{
    GetAudioEngine->pauseEffect(nSoundId);
}

void AudioHelper:: resumeSound(unsigned int nSoundId)
{
    GetAudioEngine->resumeEffect(nSoundId);
}

void AudioHelper::playMusic(const char* music, bool bLoop /*= true*/)
{
#if TARGET_IPHONE_SIMULATOR == 0
    if (mMusicVolume <= 0) {
        
        if (GetAudioEngine->isBackgroundMusicPlaying())
            GetAudioEngine->stopBackgroundMusic();
        
        return;
    }
    
    
    std::string temp(music);
    
    if (preMusicName == temp && GetAudioEngine->isBackgroundMusicPlaying())
    {
        // this music is already played.
        return;
    }
    else if(preMusicName != temp)
    {
        preMusicName = temp;
        
        if (GetAudioEngine->isBackgroundMusicPlaying())
            GetAudioEngine->stopBackgroundMusic();
    }
    
    GetAudioEngine->playBackgroundMusic(music, bLoop);
#endif
}

void AudioHelper::stopMusic()
{
    GetAudioEngine->stopBackgroundMusic();
}

void AudioHelper:: preloadMusic(const char* pszFilePath)
{
#if TARGET_IPHONE_SIMULATOR == 0
    GetAudioEngine->preloadBackgroundMusic(pszFilePath);
#endif
}

void AudioHelper::reset()
{
    mMusicVolume = DEFAULT_VOLUME;
    mSoundVolume = DEFAULT_VOLUME;
    
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    GetAudioEngine->setEffectsVolume(mSoundVolume);
}

AudioHelper::AudioHelper() : preMusicName()
{
    // load volume
    mMusicVolume = STLoadFloatFromUD(key_music_volume, DEFAULT_VOLUME);
    mSoundVolume = STLoadFloatFromUD(key_sound_volume, DEFAULT_VOLUME);
    
    GetAudioEngine->setBackgroundMusicVolume(mMusicVolume);
    GetAudioEngine->setEffectsVolume(mSoundVolume);
}

AudioHelper::~AudioHelper()
{
    STSaveFloatToUD(key_music_volume, mMusicVolume);
    STSaveFloatToUD(key_sound_volume, mSoundVolume);
}


void AudioHelper::switchSound(bool on)
{
    if (on) {
        reset();
    }
    else
    {
        changeMusicVolume(0);
        changeSoundVolume(0);
    }
    
    STSaveFloatToUD(key_music_volume, mMusicVolume);
    STSaveFloatToUD(key_sound_volume, mSoundVolume);
}
