//
//  CMAudioUtil.h
//  tangbowen
//
//  Created by tangbowen on 14-8-12.
//
//

#ifndef CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_
#define CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_
#include <iostream>
#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace std;

#define         userDefault             UserDefault::getInstance()
#define         kAudioUtil              CMAudioUtil::getInstance()

class CMAudioUtil
{
public:

public:
    static CMAudioUtil*   getInstance();
public:
    void closeAllAudio();
    void openAllAudio();
    
    void closeBgMusic();
    void openBgMusic();
    
    void closeAllEffect();
    void openAllEffect();
    
    void setBgMusicVolume(float value);
    void setEffectMusicVolume(float value);
    
    /*开关音效*/
    void switchAllAudio();
    /*通过控制音量来关闭音效*/
    void switchAllAudioByVoice();
    
    void preloadBackgroundMusic(const char *pszFilePath);
    void preloadEffect(const char *pszFilePath);
    void playBackGroundMusic(const string& pszFilePath);
    void stopBackGroundMusic();
    void exitStopAndSaveConfig();
    int playEffect(const char* pszFilePath, bool bLoop = false);
    int playEffectWithStop(const char* pszFilePath, bool bLoop, const unsigned int nSoundId);
    void stopEffectBySoundId(unsigned int nSoundId);
    void stopEffectByPath(const string& path);
    void stopAllEffect();
    void playRandomEffect(std::vector<std::string> audiosStr);
    
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    
    void loadAudioConfig();
    void saveAudioConfig();
    
    /*获取当前音效的开关状态*/
    bool getAudioState() const;
    
    string getBgMusicName() const
    {
        return _currentBgMusicName;
    }
    
    void setBgMusicName(const string& name)
    {
        _currentBgMusicName = name;
    }
    
    bool getBgMusicState() const
    {
        return _playBgMusic;
    }
    
    bool getEffectState() const
    {
        return _playEffect;
    }
    
    float getBgVolume() const
    {
        return _bgVolume;
    }
    
    float getEffectVolume() const
    {
        return _effectVolume;
    }
protected:
    bool    _allAudio;
    bool    _voiceAudio;
    bool    _playBgMusic;
    bool    _playEffect;
    string  _currentBgMusicName;
    
    float   _bgVolume;
    float   _effectVolume;
    
    SimpleAudioEngine* _audioEngine;
    
private:
    CMAudioUtil();
    virtual ~CMAudioUtil();
};

CC_DEPRECATED_ATTRIBUTE typedef CMAudioUtil AudioUtil;

#endif /* defined(CODEBASE_CORE_COMMON_CMAUDIOUTIL_H_) */
